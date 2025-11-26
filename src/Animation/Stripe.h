#pragma once
#include <Atlas/TextureAtlasSystem.h>

#include <sol/sol.hpp>

class Stripe {
public:
    Stripe(sol::table stripe);
public:
    uint32_t width_in_frames;
    uint32_t height_in_frames;
    TextureId stripe_texture;
    uint32_t x = 0;
    uint32_t y = 0;
};