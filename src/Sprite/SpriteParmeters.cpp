#include <bits/stdc++.h>

#include <Sprite/SpriteParameters.h>

SpriteParmeters::SpriteParmeters(sol::table spritesource, bool load_texture)
    :SpriteSource(spritesource, load_texture) {
    priority = parseSpritePriority(spritesource["priority"].get_or(std::string{"medium"}));
    flags = parseSpriteSourceFlags(spritesource["flags"]);
    if(spritesource["shift"].valid()) {
        shift[0] = spritesource["shift"][1].get_or(0);
        shift[1] = spritesource["shift"][2].get_or(0);
       // shift = spritesource["shift"].get_or(std::vector<int>{0, 0});
    }
    rotate_shift = spritesource["rotate_shift"].get_or(false);
    apply_special_effect = spritesource["apply_special_effect"].get_or(false);
    scale = spritesource["scale"].get_or(1);
    draw_as_shadow = spritesource["draw_as_shadow"].get_or(false);
    draw_as_glow = spritesource["draw_as_glow"].get_or(false);
    draw_as_light = spritesource["draw_as_light"].get_or(false);
    occludes_light = spritesource["occludes_light"].get_or(true);
    if(draw_as_shadow) {
        draw_as_glow = false;
        draw_as_light = false;
        occludes_light = false;
    } else if (draw_as_glow) {
        draw_as_light = false;
        occludes_light = false;
    } else if (draw_as_light) {
        occludes_light = false;
    }
    mipmap_count = spritesource["mipmap_count"].get_or(0);
    mipmap_count = std::clamp(mipmap_count, (uint8_t)0, (uint8_t)5);
    apply_runtime_tint = spritesource["apply_runtime_tint"].get_or(false);
    tint_as_overlay = spritesource["tint_as_overlay"].get_or(false);
    invert_colors = spritesource["invert_colors"].get_or(false);
    //tint = parseColor(spritesource["tint"]);
    //blend_mode = parseBlendMode(spritesource["blend_mode"].get_or(std::string{"normal"}));
    generate_sdf = spritesource["generate_sdf"].get_or(false);
    surface = parseSpriteUsageSurfaceHint(spritesource["surface"].get_or(std::string{"any"}));
    usage = parseSpriteUsageHint(spritesource["usage"].get_or(std::string{"any"}));
};