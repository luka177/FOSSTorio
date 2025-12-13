#include <Sprite/Sprite.h>

Sprite::Sprite(sol::table sprite)
    : SpriteParmeters(sprite) {
    if(sprite["layers"].valid()) {
        std::cout << "[Sprite] Valid layers sprite\n";
        sol::optional<sol::table> layers_opt = sprite["layers"];
        sol::table layers_table = layers_opt.value();
        for (auto const& [key, value] : layers_table.pairs()) {
            if (value.is<sol::table>()) {
                sol::table layer_table = value.as<sol::table>();
                Sprite layersprite = Sprite(layer_table);
                if(!layersprite.haveSprite()) {
                    throw std::runtime_error("[Sprite] Layer isnt supposed to have sub-layers!\n");
                }
                layers.emplace_back(layersprite);
            } else {
                std::cout << "[Sprite] Invalid layer value (not a table)\n";
            }
        }
        std::cout << "[Sprite] Found " << layers.size() << " layers\n";
    } else if (!haveSprite()) {
        throw std::runtime_error("[Sprite] Sprite needs texture or layers!\n");
    }
    dice_x = dice_y = sprite["dice"].get_or(0);
    if(sprite["dice_x"].valid()) {
        dice_x = sprite["dice_x"].get_or(0);
    }
    if(sprite["dice_y"].valid()) {
        dice_y = sprite["dice_y"].get_or(0);
    }
}