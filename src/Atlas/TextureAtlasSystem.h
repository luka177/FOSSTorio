#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

#include <bgfx/bgfx.h>

using TextureId = uint32_t;

struct AtlasUV {
    uint16_t atlasId;
    float u0, v0;
    float u1, v1;
};


struct UVRect {
    float u0, v0, u1, v1;
};

class TextureAtlasSystem {
public:
    static TextureAtlasSystem& getInstance() {
        static TextureAtlasSystem instance;
        return instance;
    }
    TextureId requestTexture(const std::string& path);
    void bake();

    const AtlasUV& getUV(TextureId id) const;
    bgfx::TextureHandle getAtlasTexture(uint16_t atlasId) const;

private:
    TextureAtlasSystem() = default;
    TextureAtlasSystem(const TextureAtlasSystem&) = delete;
    TextureAtlasSystem& operator=(const TextureAtlasSystem&) = delete;
    TextureAtlasSystem(TextureAtlasSystem&&) = delete;
    TextureAtlasSystem& operator=(TextureAtlasSystem&&) = delete;
    struct PendingTexture {
        std::string path;
        int width = 0;
        int height = 0;
        std::vector<unsigned char> pixels; // RGBA8
    };

    struct AtlasPage {
        uint16_t id;
        bgfx::TextureHandle handle;
        int width = 0;
        int height = 0;
    };

    std::unordered_map<std::string, TextureId> pathToId;
    std::vector<PendingTexture> textures;
    std::vector<AtlasUV> uvs;
    std::vector<AtlasPage> atlases;

    bool baked = false;
};