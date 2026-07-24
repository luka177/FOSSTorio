#include <iostream>

#include <Animation/Animation.h>
#include <Lua/LuaHelper.h>

Animation::Animation(sol::table animation)
    : AnimationParameters(animation, false) {
    if(animation["layers"].valid()) {
        std::cout << "[Animation] Valid layers animation\n";
        sol::optional<sol::table> layers_opt = animation["layers"];
        sol::table layers_table = layers_opt.value();
        for (auto const& [key, value] : layers_table.pairs()) {
            if (value.is<sol::table>()) {
                sol::table layer_table = value.as<sol::table>();
                Animation layernaim = Animation(layer_table);
                if(!layernaim.haveSprite()) {
                    throw std::runtime_error("[Animation] Layer isnt supposed to have sub-layers!\n");
                }
                layers.emplace_back(layernaim);
            } else {
                std::cout << "[Animation] Invalid layer value (not a table)\n";
            }
        }
        std::cout << "[Animation] Found " << layers.size() << " layers\n";
    } else if (animation["filename"].valid()) {
        std::string path = resolveLuaPath(animation["filename"].get<std::string>());

        if (frame_count != 1) {
            uint32_t framesPerLine = line_length != 0 ? line_length : frame_count;
            uint32_t lines = (frame_count + framesPerLine - 1) / framesPerLine;
            for (uint32_t i = 0; i < frame_count; ++i) {
                textures.push_back(TextureAtlasSystem::getInstance().requestSprite(
                    path, i % framesPerLine, i / framesPerLine, framesPerLine, lines, width, height));
            }
            sprite = textures.front();
        } else {
            sprite = TextureAtlasSystem::getInstance().requestTexture(path);
        }
    } else {
        throw std::runtime_error("[Animation] either layers or filename are needed!\n");
    }
}