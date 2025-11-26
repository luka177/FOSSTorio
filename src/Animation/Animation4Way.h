#pragma once
#include <Animation/Animation.h>
#include <sol/sol.hpp>

class Animation4Way {
public:
    Animation4Way(sol::table animation4way);
protected:
    std::optional<Animation> north;
    std::optional<Animation> north_east;
    std::optional<Animation> east;
    std::optional<Animation> south_east;
    std::optional<Animation> south;
    std::optional<Animation> south_west;
    std::optional<Animation> west;
    std::optional<Animation> north_west;
};