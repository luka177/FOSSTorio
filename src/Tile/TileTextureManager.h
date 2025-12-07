#pragma once
#include <string>

#include <Tile/TileTexture.h>
#include <Atlas/TextureAtlas.h>

#include <sol/sol.hpp>

struct TileUVInfo {
    uint16_t atlasId;
    UVRect uv;
};

struct AtlasTag {};
struct TileTag  {};

class TileTextureManager {
public:
    void loadFromLua(sol::state& lua);

    const UVRect getUV(const std::string& name, int size, int index) const;
    const UVRect getUV(int tileId, int size, int index) const;

    AtlasUV getTileUVInfo(const std::string& name, int size, int index, AtlasTag) const;
    TileUVInfo getTileUVInfo(const std::string& name, int size, int index, TileTag) const;

    const TileTexture& getTileByName(const std::string& name) const;
    const std::vector<TileTexture>& getAll() const;

private:
    std::unordered_map<std::string, int> tileNameToFixedId;
    std::vector<std::string> fixedTileNames;
    std::unordered_map<std::string, int> nameToIndex;
    std::vector<TileTexture> tileTextures;
};
