#include <EntitiesPrototypes/CraftingMachinePrototype.h>

// TBD: rewrite and move
Mirroring mirroringFromString(const std::string& s) {
    if (s == "horizontal")      return Mirroring::Horizontal;
    if (s == "vertical")        return Mirroring::Vertical;
    if (s == "diagonal-pos")    return Mirroring::DiagonlPos;
    if (s == "diagonal-neg")    return Mirroring::DiagonalNeg;
    return Mirroring::MirroringNone;
}

CraftingMachinePrototype::CraftingMachinePrototype(sol::table entity)
    : EntityWithOwnerPrototype(entity) {
    sol::object obj;

    quality_affects_energy_usage = entity["quality_affects_energy_usage"].get_or(false);

    obj = entity["crafting_speed"];
    if (obj.valid()) {
        crafting_speed = obj.as<double>();
    } else {
        throw std::runtime_error("[CraftingMachinePrototype] crafting_speed isnt optional, but is missing!\n");
    }

    module_slots = entity["module_slots"].get_or(0);
    quality_affects_module_slots = entity["quality_affects_module_slots"].get_or(false);

    sol::table tbl = entity["allowed_effects"];
    EffectTypeLimitation allowed_effects = EffectTypeLimitation::None;

    if (tbl.valid()) {
        for (auto& kv : tbl) {
            std::string s = kv.second.as<std::string>();
            allowed_effects = static_cast<EffectTypeLimitation>(
                allowed_effects | effectFromString(s)
            );
        }
    }

    show_recipe_icon = entity["show_recipe_icon"].get_or(true);
    return_ingredients_on_change = entity["return_ingredients_on_change"].get_or(true);
    draw_entity_info_icon_background = entity["draw_entity_info_icon_background"].get_or(true);
    match_animation_speed_to_activity = entity["match_animation_speed_to_activity"].get_or(true);
    show_recipe_icon_on_map = entity["show_recipe_icon_on_map"].get_or(true);
    fast_transfer_modules_into_module_slots_only = entity["fast_transfer_modules_into_module_slots_only"].get_or(false);
    ignore_output_full = entity["ignore_output_full"].get_or(false);
    if(entity["graphics_set"].valid())
        graphics_set.emplace(entity["graphics_set"].get<sol::table>());
   // if(entity["graphics_set_flipped"].valid())
   //     graphics_set_flipped.emplace(entity["graphics_set_flipped"].as<sol::table>());
    forced_symmetry = mirroringFromString(entity["forced_symmetry"].get_or(std::string{}));

    std::cout << "[CraftingMachinePrototype] Created entity prototype " << name << "\n";
}