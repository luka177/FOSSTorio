#pragma
#include <Atlas/TextureAtlasSystem.h>

#include <sol/sol.hpp>

using SpriteSizeType = int16_t;

class SpriteSource {
public:
    SpriteSource(sol::table entity);
protected:
    TextureId sprite;
    SpriteSizeType width;
    SpriteSizeType height;
    SpriteSizeType x = 0;
    SpriteSizeType y = 0;
    bool load_in_minimal_mode = false;
    bool premul_alpha = true;
    bool allow_forced_downscale = false;
};