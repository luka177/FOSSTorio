#pragma once
#include <string>
#include <unordered_map>

#include <Tile/TileVariation.h>
#include <Atlas/TextureAtlas.h>

#include <sol/sol.hpp>

class TileTexture {
public:
    bool loadFromLua(sol::table tileDef);

    const std::string& getName() const;
    const TileVariation* getVariation(int size) const;

private:
    std::string name;
    std::string type;
    std::string order;
    int layer = 0;
    std::string subgroup;
    std::unordered_map<int, TileVariation> variations;
};
