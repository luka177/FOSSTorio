#include <Sprite/SpriteSource.h>
#include <Lua/LuaHelper.h>

SpriteSource::SpriteSource(sol::table spritesource) {
    sol::table t;
    bool textureless = false;
    std::cerr << "[SpriteSource] enter constructor\n";

    if (spritesource["filename"].valid()) {
        std::cerr << "[SpriteSource] have filename: " << spritesource["filename"].get<std::string>() << "\n";
        sprite = TextureAtlasSystem::getInstance().requestTexture(resolveLuaPath(spritesource["filename"].get<std::string>()));
    } else if (spritesource["layers"].valid()) {  // As per https://lua-api.factorio.com/latest/types/Animation.html#filename as there is no textrue dont try getting size
        std::cerr << "[SpriteSource] textureless, layered sprite detected\n";
        textureless = true;
    } else {
        throw std::runtime_error("[SpriteSource] either layers or filename are needed!\n");
    }

    if(!textureless) {
        width = height = -1;
        if(spritesource["size"].valid()) {
            if(spritesource["size"].is<int>()) {
                width = height = spritesource["size"].get_or(0);
            } else if (spritesource["size"].is<sol::table>()) {
                t = spritesource["size"].get<sol::table>();
                if (t[1].valid() && t[1].is<int>() && t[2].valid() && t[2].is<int>()) {
                    width = t[1].get_or(0);
                    height =  t[2].get_or(0);
                }

            }
        }

        if(width == -1) {
            width = spritesource["width"].get_or(-1);
            height = spritesource["height"].get_or(-1);
        }

        if(width < 0 || width > 4096 || height < 0 || height > 4096) {
            throw std::runtime_error("[SpriteSource] size or width and height are not optional, and should be within 0-4096 range!\n");
        }

        x = spritesource["x"].get_or(0);
        y = spritesource["y"].get_or(0);

        if(!x && !y && spritesource["position"].valid()) {
            t = spritesource["position"].get<sol::table>();
            x = t[1].get_or(0);
            y = t[2].get_or(0);
        }
    };

    load_in_minimal_mode = spritesource["load_in_minimal_mode"].get_or(false);
    premul_alpha = spritesource["premul_alpha"].get_or(true);
    allow_forced_downscale = spritesource["allow_forced_downscale"].get_or(true);
}