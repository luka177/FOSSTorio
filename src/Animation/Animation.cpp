#include <Animation/Animation.h>

Animation::Animation(sol::table animation)
    : AnimationParameters(animation) {
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
    }
}