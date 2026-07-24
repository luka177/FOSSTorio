#include <iostream>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <limits>

#include <Atlas/TextureAtlasSystem.h>

#include <bgfx/bimg/3rdparty/stb/stb_image.h>

#define ATLAS_DEBUG 1
#ifdef ATLAS_DEBUG
    #define ATLAS_LOG(expr) do { std::cerr << expr << std::endl; } while (0)
#else
    #define ATLAS_LOG(expr) do { } while (0)
#endif

bool writeBmp32(const char* filename, int width, int height, const uint8_t* rgbaData)
{
    if (!filename || !rgbaData || width <= 0 || height <= 0)
        return false;

    const int bytesPerPixel = 4;
    const int rowSize       = width * bytesPerPixel;
    const uint32_t dataSize = rowSize * height;
    const uint32_t fileSize = 14 + 40 + dataSize;

    std::ofstream out(filename, std::ios::binary);
    if (!out)
        return false;

    uint8_t bmpFileHeader[14] = {
        'B','M',
        0,0,0,0,
        0,0,
        0,0,
        54,0,0,0
    };

    bmpFileHeader[2] = (uint8_t)(fileSize      );
    bmpFileHeader[3] = (uint8_t)(fileSize >> 8 );
    bmpFileHeader[4] = (uint8_t)(fileSize >> 16);
    bmpFileHeader[5] = (uint8_t)(fileSize >> 24);

    out.write((char*)bmpFileHeader, 14);

    uint8_t bmpInfoHeader[40] = {0};
    uint32_t headerSize = 40;
    memcpy(bmpInfoHeader + 0,  &headerSize, 4);
    memcpy(bmpInfoHeader + 4,  &width,      4);
    memcpy(bmpInfoHeader + 8,  &height,     4);

    bmpInfoHeader[12] = 1;
    bmpInfoHeader[14] = 32;

    memcpy(bmpInfoHeader + 20, &dataSize, 4);

    out.write((char*)bmpInfoHeader, 40);

    for (int y = height - 1; y >= 0; --y)
    {
        const uint8_t* row = rgbaData + y * rowSize;
        for (int x = 0; x < width; x++)
        {
            uint8_t r = row[x*4 + 0];
            uint8_t g = row[x*4 + 1];
            uint8_t b = row[x*4 + 2];
            uint8_t a = row[x*4 + 3];

            uint8_t bgra[4] = { b, g, r, a };
            out.write((char*)bgra, 4);
        }
    }

    return true;
}

// Skyline (bottom-left) bin packer: keeps a contour of the tallest occupied point per
// x-range, so shorter images can slot in under taller neighbours instead of wasting
// the rest of a shelf's row height, unlike naive shelf packing.
struct SkylineNode {
    int x;
    int y;
    int width;
};

static bool skylineRectFits(const std::vector<SkylineNode>& skyline, std::size_t index,
                             int w, int h, int atlasW, int atlasH, int& outY) {
    int x = skyline[index].x;
    if (x + w > atlasW) return false;

    int widthLeft = w;
    std::size_t i = index;
    int y = 0;
    while (widthLeft > 0) {
        y = std::max(y, skyline[i].y);
        if (y + h > atlasH) return false;
        widthLeft -= skyline[i].width;
        ++i;
        if (widthLeft > 0 && i == skyline.size()) return false;
    }

    outY = y;
    return true;
}

static bool skylineFindPosition(const std::vector<SkylineNode>& skyline, int w, int h,
                                 int atlasW, int atlasH, int& bestX, int& bestY, std::size_t& bestIndex) {
    int bestWidth = std::numeric_limits<int>::max();
    bestY = std::numeric_limits<int>::max();
    bool found = false;

    for (std::size_t i = 0; i < skyline.size(); ++i) {
        int y;
        if (skylineRectFits(skyline, i, w, h, atlasW, atlasH, y)) {
            if (y < bestY || (y == bestY && skyline[i].width < bestWidth)) {
                bestY = y;
                bestX = skyline[i].x;
                bestWidth = skyline[i].width;
                bestIndex = i;
                found = true;
            }
        }
    }

    return found;
}

static void skylineAddLevel(std::vector<SkylineNode>& skyline, std::size_t index,
                             int x, int y, int w, int h) {
    skyline.insert(skyline.begin() + index, SkylineNode{x, y + h, w});

    for (std::size_t i = index + 1; i < skyline.size(); ) {
        SkylineNode& prev = skyline[i - 1];
        SkylineNode& cur = skyline[i];

        if (cur.x >= prev.x + prev.width) break;

        int shrink = prev.x + prev.width - cur.x;
        cur.x += shrink;
        cur.width -= shrink;

        if (cur.width <= 0) {
            skyline.erase(skyline.begin() + i);
        } else {
            break;
        }
    }

    for (std::size_t i = 0; i + 1 < skyline.size(); ) {
        if (skyline[i].y == skyline[i + 1].y) {
            skyline[i].width += skyline[i + 1].width;
            skyline.erase(skyline.begin() + i + 1);
        } else {
            ++i;
        }
    }
}

TextureId TextureAtlasSystem::requestSprite(
    const std::string& path,
    int col, int row,
    int cols, int rows,
    int width, int height,
    int paddingX,
    int paddingY
) {
    TextureId baseId = requestTexture(path);
    TextureId id = static_cast<TextureId>(textures.size());
    uvs.push_back({0, 0.0f, 0.0f, 0.0f, 0.0f});

    PendingSprite tex;
    tex.baseTexId = baseId;
    tex.col = col;
    tex.row = row;
    tex.cols = cols;
    tex.rows = rows;
    tex.width = width;
    tex.height = height;
    tex.paddingX = paddingX;
    tex.paddingY = paddingY;

    textures.push_back(tex);

    ATLAS_LOG("requestSprite: \"" << path << "\" sprite ("
               << col << "," << row << ") of "
               << cols << "x" << rows
               << " -> id " << id << " (base " << baseId << ")");

    return id;
}

TextureId TextureAtlasSystem::requestTexture(const std::string& path) {
    auto it = pathToId.find(path);
    if (it != pathToId.end()) {
        ATLAS_LOG("requestTexture: reuse \"" << path << "\" -> id " << it->second);
        return it->second;
    }

    TextureId id = static_cast<TextureId>(textures.size());
    pathToId[path] = id;

    PendingSprite tex;
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
        if (tex.path.empty()) continue;

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
        if (tex.path.empty()) continue;
        if (tex.width == 0 || tex.height == 0) continue;
        entries.push_back({id, tex.width, tex.height});
    }

    ATLAS_LOG("Images to pack: " << entries.size());
    // Skyline packing works best fed tallest-first
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

    // All pages stay open for the whole pass: a texture that doesn't fit the newest
    // page can still land in a gap of an earlier one. A page is only ever read as
    // "full" for the one texture that didn't fit it, not closed off for good -
    // otherwise every texture after the first miss piles onto fresh pages while
    // older ones sit half-empty.
    struct Placement {
        TextureId id;
        int page;
        int x;
        int y;
    };

    std::vector<std::vector<SkylineNode>> pageSkylines;
    std::vector<Placement> placements;

    for (const auto& e : entries) {
        auto& tex = textures[e.id];

        if (tex.width > atlasSize || tex.height > atlasSize) {
            std::cerr << "[Atlas] Texture too large for atlas: " << tex.path
                      << " (" << tex.width << "x" << tex.height << ")\n";
            continue;
        }

        // First-fit across pages, oldest first: comparing y globally would systematically
        // favour whichever page is newest (a barely-used page offers a low y almost
        // everywhere), starving genuine gaps in earlier pages that just happen to sit a
        // bit higher up. Always give earlier pages first crack at every texture instead.
        int bestPage = -1, bestX = 0, bestY = 0;
        std::size_t bestIndex = 0;

        for (std::size_t p = 0; p < pageSkylines.size(); ++p) {
            int x, y;
            std::size_t idx;
            if (skylineFindPosition(pageSkylines[p], tex.width, tex.height, atlasSize, atlasSize, x, y, idx)) {
                bestY = y;
                bestX = x;
                bestPage = static_cast<int>(p);
                bestIndex = idx;
                break;
            }
        }

        if (bestPage < 0) {
            pageSkylines.push_back({ SkylineNode{0, 0, atlasSize} });
            bestPage = static_cast<int>(pageSkylines.size()) - 1;
            if (!skylineFindPosition(pageSkylines[bestPage], tex.width, tex.height, atlasSize, atlasSize,
                                      bestX, bestY, bestIndex)) {
                std::cerr << "[Atlas] Failed to place texture on a fresh page: " << tex.path << "\n";
                continue;
            }
        }

        skylineAddLevel(pageSkylines[bestPage], bestIndex, bestX, bestY, tex.width, tex.height);
        placements.push_back({ e.id, bestPage, bestX, bestY });

        ATLAS_LOG("Placed id " << e.id << " \"" << tex.path << "\""
                  << " on page " << bestPage
                  << " at (" << bestX << ", " << bestY << ")"
                  << " size (" << tex.width << "x" << tex.height << ")");
    }

    // Only now that every texture has a final (page, x, y) do we rasterize and upload,
    // one page at a time, so a page's pixel buffer is only ever needed transiently.
    std::vector<uint8_t> atlasPixels(size_t(atlasSize) * size_t(atlasSize) * 4, 0);

    for (std::size_t p = 0; p < pageSkylines.size(); ++p) {
        std::fill(atlasPixels.begin(), atlasPixels.end(), 0);
        usedPixelsThisPage = 0;

        for (const auto& pl : placements) {
            if (pl.page != static_cast<int>(p)) continue;

            const auto& tex = textures[pl.id];
            usedPixelsThisPage += size_t(tex.width) * size_t(tex.height);

            for (int iy = 0; iy < tex.height; ++iy) {
                for (int ix = 0; ix < tex.width; ++ix) {
                    int dst = ((pl.y + iy) * atlasSize + (pl.x + ix)) * 4;
                    int src = (iy * tex.width + ix) * 4;
                    std::memcpy(&atlasPixels[dst], &tex.pixels[src], 4);
                }
            }

            AtlasUV& uv = uvs[pl.id];
            uv.atlasId = static_cast<uint16_t>(p);
            uv.u0 = static_cast<float>(pl.x) / atlasSize;
            uv.v0 = static_cast<float>(pl.y) / atlasSize;
            uv.u1 = static_cast<float>(pl.x + tex.width) / atlasSize;
            uv.v1 = static_cast<float>(pl.y + tex.height) / atlasSize;
        }

        size_t totalPixels = size_t(atlasSize) * size_t(atlasSize);
        float efficiency = totalPixels > 0 ? float(usedPixelsThisPage) / float(totalPixels) : 0.0f;
        ATLAS_LOG("Atlas page " << p << " efficiency: " << (efficiency * 100.0f) << "% "
                  << "(" << usedPixelsThisPage << "/" << totalPixels << " px)");

        #ifdef ATLAS_DEBUG
        {
            std::string debugFilename = "debug_atlas_page_" + std::to_string(p) + ".bmp";
            writeBmp32(debugFilename.c_str(), atlasSize, atlasSize, atlasPixels.data());
            ATLAS_LOG("Dumped atlas page " << p << " to " << debugFilename);
        }
        #endif

        const bgfx::Memory* mem = bgfx::copy(atlasPixels.data(), atlasPixels.size());
        bgfx::TextureHandle handle = bgfx::createTexture2D(
            (uint16_t)atlasSize, (uint16_t)atlasSize,
            false, 1, bgfx::TextureFormat::RGBA8,
            BGFX_SAMPLER_POINT, mem
        );

        AtlasPage page;
        page.id = static_cast<uint16_t>(p);
        page.handle = handle;
        page.width = atlasSize;
        page.height = atlasSize;
        atlases.push_back(page);

        ATLAS_LOG("Created atlas page " << p << " (" << atlasSize << "x" << atlasSize << ")");
    }

    for (std::size_t i = 0; i < textures.size(); ++i) {
    const auto& s = textures[i];
        if(!s.path.empty())
            continue;
        if (s.baseTexId >= textures.size()) {
            std::cerr << "[Atlas] Pending sprite baseTexId out of range\n";
            continue;
        }

        const auto& baseTex = textures[s.baseTexId];
        const AtlasUV& baseUv = uvs[s.baseTexId];

        if (baseTex.width == 0 || baseTex.height == 0) {
            std::cerr << "[Atlas] Pending sprite refers to zero-size base texture: "
                      << baseTex.path << "\n";
            continue;
        }

        int sheetW = baseTex.width;
        int sheetH = baseTex.height;

        int cellW = s.width;
        int cellH = s.height;

        int x_px = s.col * (cellW + s.paddingX);
        int y_px = s.row * (cellH + s.paddingY);

        if (x_px + cellW > sheetW || y_px + cellH > sheetH) {
            std::cerr << "[Atlas] Sprite out of range in sheet: "
                      << baseTex.path << " (col=" << s.col
                      << ", row=" << s.row << ")\n";
            continue;
        }

        float relU0 = float(x_px)         / float(sheetW);
        float relV0 = float(y_px)         / float(sheetH);
        float relU1 = float(x_px + cellW) / float(sheetW);
        float relV1 = float(y_px + cellH) / float(sheetH);

        AtlasUV uv;
        uv.atlasId = baseUv.atlasId;
        uv.u0 = baseUv.u0 + (baseUv.u1 - baseUv.u0) * relU0;
        uv.v0 = baseUv.v0 + (baseUv.v1 - baseUv.v0) * relV0;
        uv.u1 = baseUv.u0 + (baseUv.u1 - baseUv.u0) * relU1;
        uv.v1 = baseUv.v0 + (baseUv.v1 - baseUv.v0) * relV1;


        uvs[i] = uv;

        ATLAS_LOG("Sprite UV for id " << i
                  << " from base " << s.baseTexId
                  << " (col=" << s.col << ", row=" << s.row << "): "
                  << "[" << uv.u0 << "," << uv.v0
                  << " to " << uv.u1 << "," << uv.v1 << "]"
                  << " atlas page " << uv.atlasId);
    }

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
