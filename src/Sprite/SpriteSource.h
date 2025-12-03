#pragma
#include <Atlas/TextureAtlasSystem.h>

#include <sol/sol.hpp>

using SpriteSizeType = int16_t;

class SpriteSource {
public:
    SpriteSource(sol::table entity, bool load_texture = true);
    const TextureId getSprite() const {
        if (!sprite) {
            throw std::runtime_error("[SpriteSource] This is spritles layer container layer, DO NOT TRY TO ACCESS SPRITE!\n");
        }
        return *sprite;
    }

    bool haveSprite() {
        if (!sprite) {
            return false;
        } else {
            return true;
        }
    }

    SpriteSizeType getWidth() {
        return width;
    }

    SpriteSizeType getHeight() {
        return height;
    }
protected:
    std::optional<TextureId> sprite;
    SpriteSizeType width;
    SpriteSizeType height;
    SpriteSizeType x = 0;
    SpriteSizeType y = 0;
    bool load_in_minimal_mode = false;
    bool premul_alpha = true;
    bool allow_forced_downscale = false;
};