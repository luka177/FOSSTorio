#pragma once
#include <vector>

#include <Sprite/SpriteParameters.h>
#include <Sprite/SpriteTypes.h>

#include <sol/sol.hpp>

class SpriteNWaySheet : public SpriteParmeters {
public:
    SpriteNWaySheet(sol::table spritesource);

protected:
    uint32_t frames = 4;
};