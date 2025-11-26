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
                layers.emplace_back(Animation(layer_table));
            } else {
                std::cout << "[Animation] Invalid layer value (not a table)\n";
            }
        }
        std::cout << "[Animation] Found " << layers.size() << " layers\n";
    }
}