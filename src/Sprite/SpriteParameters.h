#pragma once
#include <vector>

#include <Sprite/SpriteSource.h>
#include <Sprite/SpriteTypes.h>

#include <sol/sol.hpp>

class SpriteParmeters : public SpriteSource {
public:
    SpriteParmeters(sol::table spritesource, bool load_texture = true);
    double getScale() {
        return scale;
    }

    bool getDrawAsShadow() {
        return draw_as_shadow;
    }
protected:
    SpritePriority priority = SpritePriority::Medium;
    SpriteFlags flags;
    std::vector<int> shift{0, 0};
    bool rotate_shift = false;
    bool apply_special_effect = false;
    double scale = 1;
    // Only one of draw_as_shadow, draw_as_glow and draw_as_light can be true. This takes precedence over draw_as_glow and draw_as_light.
    bool draw_as_shadow = false;
    bool draw_as_glow = false;
    bool draw_as_light = false;
    // Loaded only if draw_as_shadow, draw_as_glow and draw_as_light are false, and only by sprites used by tile renderer (decals and underwater patches). The purpose of setting this to false is to preserve water mask from sprites that are supposed to be drawn under the water.
    bool occludes_light = true;
    uint8_t mipmap_count = 0;
    bool apply_runtime_tint = false;
    bool tint_as_overlay = false;
    bool invert_colors = false;
    //Color tint;
    //BlendMode blend_mode = BlendMode::Normal;
    bool generate_sdf = false;
    SpriteUsageSurfaceHint surface = SpriteUsageSurfaceHint::Any;
    SpriteUsageHint usage = SpriteUsageHint::Any;
};