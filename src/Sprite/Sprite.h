#pragma once
#include <vector>

#include <Sprite/SpriteParameters.h>
#include <Sprite/SpriteTypes.h>

#include <sol/sol.hpp>

class Sprite : public SpriteParmeters {
public:
    Sprite(sol::table spritesource);

protected:
    std::vector<Sprite> layers;
    SpriteSizeType dice;
    SpriteSizeType dice_x;
    SpriteSizeType dice_y;
};