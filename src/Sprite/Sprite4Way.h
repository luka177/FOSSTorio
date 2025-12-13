#pragma once
#include <Atlas/TextureAtlasSystem.h>
#include <Sprite/Sprite.h>
#include <Sprite/SpriteNWaySheet.h>

#include <sol/sol.hpp>

class Sprite4Way {
public:
    Sprite4Way(sol::table sprite);

protected:
    //std::vector<SpriteNWaySheet> sheets;
    std::optional<SpriteNWaySheet> sheet;
    std::optional<Sprite> north;
    std::optional<Sprite> east;
    std::optional<Sprite> south;
    std::optional<Sprite> west;
};