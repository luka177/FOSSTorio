#pragma once

#include <string>

#include <Atlas/TextureAtlas.h>
#include <Atlas/TextureAtlasSystem.h>

#include <sol/sol.hpp>

class TileVariation {
public:
    TileVariation();

    bool loadFromLua(sol::table variant);

    UVRect getUV(int index) const;

    int getCount() const;
    int getSize() const;
    TextureId getTextureId() const;

private:
    TextureId textureId;
    int count;
    int size;
    int lineLength;
    int yOffset;
    float scale;
};
