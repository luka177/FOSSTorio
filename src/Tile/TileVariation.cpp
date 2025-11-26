#include <iostream>

#include <Tile/TileVariation.h>
#include <Lua/LuaHelper.h>

TileVariation::TileVariation()
    : textureId(-1), count(16), size(1), lineLength(16), yOffset(0), scale(1.0f) {}

bool TileVariation::loadFromLua(sol::table variant) {
    sol::object picObj = variant["picture"];
    if (!picObj.valid()) {
        std::cerr << "[TileVariation] Missing picture field.\n";
        return false;
    }

    std::string virtualPath = picObj.as<std::string>();
    std::string resolved = resolveLuaPath(virtualPath);

    textureId = TextureAtlasSystem::getInstance().requestTexture(resolved);

    count      = variant.get_or("count", 16);
    size       = variant.get_or("size", 1);
    lineLength = variant.get_or("line_length", 16);
    yOffset    = variant.get_or("y", 0);
    scale      = variant.get_or("scale", 1.0f);

    return true;
}

UVRect TileVariation::getUV(int index) const {
    if (textureId < 0) {
        return UVRect{0, 0, 0, 0};
    }

    const AtlasUV& base = TextureAtlasSystem::getInstance().getUV(textureId);

    int tileSizePx = size * 64;
    int tilesPerRow = lineLength;

    int ix = index % tilesPerRow;
    int iy = index / tilesPerRow;

    //TBD: Get from AtlasManager
    int atlasW = 8192;
    int atlasH = 8192;

    float tileW = static_cast<float>(tileSizePx) / static_cast<float>(atlasW);
    float tileH = static_cast<float>(tileSizePx) / static_cast<float>(atlasH);

    float offsetU = static_cast<float>(ix * tileSizePx) / static_cast<float>(atlasW);
    float offsetV = static_cast<float>(yOffset + iy * tileSizePx) / static_cast<float>(atlasH);

    return UVRect{
        base.u0 + offsetU,
        base.v0 + offsetV,
        base.u0 + offsetU + tileW,
        base.v0 + offsetV + tileH
    };
}

int TileVariation::getCount() const {
    return count;
}

int TileVariation::getSize() const {
    return size;
}

TextureId TileVariation::getTextureId() const {
    return textureId;
}