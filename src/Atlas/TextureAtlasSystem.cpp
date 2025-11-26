#include <iostream>
#include <algorithm>
#include <cstring>

#include <Atlas/TextureAtlasSystem.h>

#include <bgfx/bimg/3rdparty/stb/stb_image.h>

#define ATLAS_DEBUG 1
#ifdef ATLAS_DEBUG
    #define ATLAS_LOG(expr) do { std::cerr << expr << std::endl; } while (0)
#else
    #define ATLAS_LOG(expr) do { } while (0)
#endif

TextureId TextureAtlasSystem::requestTexture(const std::string& path) {
    auto it = pathToId.find(path);
    if (it != pathToId.end()) {
        ATLAS_LOG("requestTexture: reuse \"" << path << "\" -> id " << it->second);
        return it->second;
    }

    TextureId id = static_cast<TextureId>(textures.size());
    pathToId[path] = id;

    PendingTexture tex;
    tex.path = path;
    textures.push_back(std::move(tex));
    uvs.push_back({0, 0, 0, 0, 0}); // placeholder

    baked = false;

    ATLAS_LOG("requestTexture: new \"" << path << "\" -> id " << id);

    return id;
}

void TextureAtlasSystem::bake() {
    if (baked) {
        ATLAS_LOG("bake() called but already baked; skipping");
        return;
    }

    size_t totalImageArea = 0;
    size_t usedPixelsThisPage = 0;
    ATLAS_LOG("bake() starting, textures: " << textures.size());

    //get all images
    for (auto& tex : textures) {
        if (!tex.pixels.empty()) continue;

        int w, h, ch;
        unsigned char* img = stbi_load(tex.path.c_str(), &w, &h, &ch, 4);
        if (!img) {
            std::cerr << "[Atlas] Failed to load " << tex.path << "\n";
            continue;
        }
        tex.width = w;
        tex.height = h;
        totalImageArea += size_t(w) * size_t(h);
        tex.pixels.assign(img, img + w * h * 4);
        stbi_image_free(img);
        ATLAS_LOG("Loaded \"" << tex.path
                  << " size: " << w << "x" << h);
    }

    //list of evrything
    struct ImageEntry {
        TextureId id;
        int width;
        int height;
    };

    std::vector<ImageEntry> entries;
    for (TextureId id = 0; id < textures.size(); ++id) {
        const auto& tex = textures[id];
        if (tex.width == 0 || tex.height == 0) continue;
        entries.push_back({id, tex.width, tex.height});
    }

    ATLAS_LOG("Images to pack: " << entries.size());
    //TBD: SMART packing
    // For now sort by height, as it allows to get slightly denser packing

    std::sort(entries.begin(), entries.end(), [](const ImageEntry& a, const ImageEntry& b) {
        if (a.height != b.height) return a.height > b.height;
        return a.id < b.id;
    });

    // actually pack
    const bgfx::Caps* caps = bgfx::getCaps();
    int maxTexSize = caps->limits.maxTextureSize;
    int atlasSize = std::min(8192, maxTexSize); // TBD: Set based on gpu caps

    ATLAS_LOG("Device maxTextureSize = " << maxTexSize
              << ", using atlasSize = " << atlasSize);

    uint16_t currentAtlasId = 0;
    int x = 0, y = 0, rowH = 0;
    std::vector<uint8_t> atlasPixels(atlasSize * atlasSize * 4, 0);

    auto flushAtlas = [&]() {
        if (currentAtlasId > 0 || x > 0 || y > 0) {
            size_t totalPixels = size_t(atlasSize) * size_t(atlasSize);
            float efficiency = totalPixels > 0
                ? float(usedPixelsThisPage) / float(totalPixels)
                : 0.0f;

            ATLAS_LOG("Atlas page " << currentAtlasId << " efficiency: "
                    << (efficiency * 100.0f) << "% "
                    << "(" << usedPixelsThisPage << "/" << totalPixels << " px)");

            usedPixelsThisPage = 0;   // reset for next page

            // upload current page
            #ifdef ATLAS_DEBUG
            {
                std::string debugFilename = "debug_atlas_page_" + std::to_string(currentAtlasId) + ".bmp";
               // stbi_write_bmp(debugFilename.c_str(), atlasSize, atlasSize, 4, atlasPixels.data());
                ATLAS_LOG("Dumped atlas page " << currentAtlasId << " to " << debugFilename);
            }
            #endif
            const bgfx::Memory* mem = bgfx::copy(atlasPixels.data(), atlasPixels.size());
            bgfx::TextureHandle handle = bgfx::createTexture2D(
                (uint16_t)atlasSize, (uint16_t)atlasSize,
                false, 1, bgfx::TextureFormat::RGBA8,
                BGFX_SAMPLER_POINT, mem
            );

            AtlasPage page;
            page.id = currentAtlasId;
            page.handle = handle;
            page.width = atlasSize;
            page.height = atlasSize;
            atlases.push_back(page);

            ATLAS_LOG("Created atlas page " << currentAtlasId
                  << " (" << atlasSize << "x" << atlasSize << ")");

            currentAtlasId++;
            std::fill(atlasPixels.begin(), atlasPixels.end(), 0);
            x = y = rowH = 0;
        }
    };

    flushAtlas();

    for (const auto& e : entries) {
        auto& tex = textures[e.id];

        // a new row if needed
        if (x + tex.width > atlasSize) {
            x = 0;
            y += rowH;
            rowH = 0;
        }

        // a new atlas if needed
        if (y + tex.height > atlasSize) {
            ATLAS_LOG("Page " << currentAtlasId << " full, flushing and starting new");
            flushAtlas();
        }

        if (tex.width > atlasSize || tex.height > atlasSize) {
            std::cerr << "[Atlas] Texture too large for atlas: " << tex.path
                      << " (" << tex.width << "x" << tex.height << ")\n";
            continue;
        }

        // Blit into current atlas
        usedPixelsThisPage += size_t(tex.width) * size_t(tex.height);
        for (int iy = 0; iy < tex.height; ++iy) {
            for (int ix = 0; ix < tex.width; ++ix) {
                int dst = ((y + iy) * atlasSize + (x + ix)) * 4;
                int src = (iy * tex.width + ix) * 4;
                std::memcpy(&atlasPixels[dst], &tex.pixels[src], 4);
            }
        }

        // store UVs
        AtlasUV& uv = uvs[e.id];
        uv.atlasId = currentAtlasId;
        uv.u0 = static_cast<float>(x) / atlasSize;
        uv.v0 = static_cast<float>(y) / atlasSize;
        uv.u1 = static_cast<float>(x + tex.width) / atlasSize;
        uv.v1 = static_cast<float>(y + tex.height) / atlasSize;

        ATLAS_LOG("Placed id " << e.id << " \"" << tex.path << "\""
                  << " on page " << currentAtlasId
                  << " at (" << x << ", " << y << ")"
                  << " size (" << tex.width << "x" << tex.height << ")"
                  << " UV [" << uv.u0 << ", " << uv.v0
                  << " → " << uv.u1 << ", " << uv.v1 << "]");

        x += tex.width;
        rowH = std::max(rowH, tex.height);
    }

    // The last atlas
    flushAtlas();

    size_t atlasPageArea = size_t(atlasSize) * size_t(atlasSize);
    size_t totalAtlasArea = atlasPageArea * atlases.size();

    float efficiency = totalAtlasArea > 0
        ? float(totalImageArea) / float(totalAtlasArea)
        : 0.0f;

    ATLAS_LOG("Atlas packing efficiency: "
            << (efficiency * 100.0f) << "% "
            << "(" << totalImageArea << " px used of "
            << totalAtlasArea << " px)");

    ATLAS_LOG("bake() finished, pages: " << atlases.size());

    baked = true;
}

bgfx::TextureHandle TextureAtlasSystem::getAtlasTexture(uint16_t atlasId) const {
    return atlases[atlasId].handle;
}

const AtlasUV& TextureAtlasSystem::getUV(TextureId id) const {
    static AtlasUV dummy{0, 0.0f, 0.0f, 0.0f, 0.0f};

    if (id >= uvs.size()) {
        std::cerr << "[TextureAtlasSystem] getUV(" << id << ") out of range\n";
        return dummy;
    }

    return uvs[id];
}
