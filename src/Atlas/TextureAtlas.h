#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

#include <Atlas/TextureAtlasSystem.h>

#include <sol/sol.hpp>
#include <bgfx/bgfx.h>

struct ImageEntry {
    std::string path;
    int width, height;
    unsigned char* data;
};

class TextureAtlas {
public:
    struct UVRect {
        float u0, v0, u1, v1;
    };

    struct Entry {
        int id;
        std::string name;
        UVRect uv;
    };

    bool buildFromLua(sol::state& lua, const std::string& entityType);
    const UVRect& getUV(int id) const;
    int getTextureId(const std::string& name) const;
    const std::string& getTextureName(int id) const;
    bgfx::TextureHandle getAtlasTexture() const;
    int getAtlasHeight() const;
    int getAtlasWidth() const;

private:
    bgfx::TextureHandle atlasHandle = BGFX_INVALID_HANDLE;
    std::unordered_map<std::string, int> nameToId;
    std::vector<std::string> idToName;
    std::unordered_map<int, UVRect> idToUV;
    std::vector<ImageEntry> images;

    int atlasWidth, atlasHeight;
    int nextId = 0;
};
