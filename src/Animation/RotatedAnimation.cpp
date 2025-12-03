#include <Animation/RotatedAnimation.h>
#include <Lua/LuaHelper.h>
#include <sol/sol.hpp>

RotatedAnimation::RotatedAnimation(sol::table animation)
    : AnimationParameters(animation, false) {
    direction_count = animation["direction_count"].get_or(1);
    lines_per_file = animation["lines_per_file"].get_or(1);
    if(animation["layers"].valid()) {
        std::cout << "[RotatedAnimation] Valid layers animation\n";
        sol::optional<sol::table> layers_opt = animation["layers"];
        sol::table layers_table = layers_opt.value();
        for (auto const& [key, value] : layers_table.pairs()) {
            if (value.is<sol::table>()) {
                sol::table layer_table = value.as<sol::table>();
                RotatedAnimation layernaim = RotatedAnimation(layer_table);
                if(!layernaim.haveSprite()) {
                    throw std::runtime_error("[RotatedAnimation] Layer isnt supposed to have sub-layers!\n");
                }
                layers.emplace_back(layernaim);
            } else {
                std::cout << "[RotatedAnimation] Invalid layer value (not a table)\n";
            }
        }
        std::cout << "[RotatedAnimation] Found " << layers.size() << " layers\n";
    } else {
        if(direction_count != 1 || frame_count != 1) {
            for(int i = 0; i < direction_count; i++) {
                for(int ii = 0; ii < frame_count; ii++) {
                    if(i == ii == 0) {
                        sprite = TextureAtlasSystem::getInstance().requestSprite(resolveLuaPath(animation["filename"].get<std::string>()), ii, i, frame_count, direction_count, width, height);
                    }
                    sprites.push_back(TextureAtlasSystem::getInstance().requestSprite(resolveLuaPath(animation["filename"].get<std::string>()), ii, i, frame_count, direction_count, width, height));
                }
            }
        } else {
            sprite = TextureAtlasSystem::getInstance().requestTexture(resolveLuaPath(animation["filename"].get<std::string>()));
        }
    }
}