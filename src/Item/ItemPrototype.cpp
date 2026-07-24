#include <Item/ItemPrototype.h>

#include <Lua/LuaHelper.h>

ItemPrototype::ItemPrototype(sol::table entity)
    : Prototype(entity) {
    if (entity["stack_size"].valid()) {
        stack_size = entity["stack_size"].get<uint32_t>();
    } else {
        throw std::runtime_error("[ItemPrototype] stack_size isnt optional, but is missing!\n");
    }

    icon_size = entity["icon_size"].get_or(64);
    if (entity["icon"].valid()) {
        icon = TextureAtlasSystem::getInstance().requestSprite(resolveLuaPath(entity["icon"].get<std::string>()), 0, 0, 1, 1, icon_size, icon_size);
    }

    place_result = entity["place_result"].get_or(std::string{});
    fuel_category = entity["fuel_category"].get_or(std::string{});
    burnt_result = entity["burnt_result"].get_or(std::string{});
    spoil_result = entity["spoil_result"].get_or(std::string{});
    spoil_quality_change = entity["spoil_quality_change"].get_or(0);
    plant_result = entity["plant_result"].get_or(std::string{});
    flags = parseItemFlags(entity["flags"]);
    spoil_ticks = entity["spoil_ticks"].get_or(0);

    sol::object fuelValue = entity["fuel_value"];
    if (fuelValue.valid()) {
        fuel_value = fuelValue.is<std::string>() ? parseEnergy(fuelValue.as<std::string>()) : fuelValue.as<double>();
    }

    fuel_acceleration_multiplier = entity["fuel_acceleration_multiplier"].get_or(1.0);
    fuel_top_speed_multiplier = entity["fuel_top_speed_multiplier"].get_or(1.0);
    fuel_emissions_multiplier = entity["fuel_emissions_multiplier"].get_or(1.0);
    weight = entity["weight"].get_or(0.0);
    ingredient_to_weight_coefficient = entity["ingredient_to_weight_coefficient"].get_or(0.5);
    space_platform_request_priority = entity["space_platform_request_priority"].get_or(false);
    default_import_location = entity["default_import_location"].get_or(std::string("nauvis"));
    has_random_tint = entity["has_random_tint"].get_or(true);
    send_to_orbit_mode = entity["send_to_orbit_mode"].get_or(std::string("not-sendable"));
    moved_to_hub_when_building = entity["moved_to_hub_when_building"].get_or(false);
    spoil_level = entity["spoil_level"].get_or(0);
    auto_recycle = entity["auto_recycle"].get_or(true);
    lab_ignores_spoil_percent = entity["lab_ignores_spoil_percent"].get_or(false);

    std::cout << "[ItemPrototype] Created item prototype " << name << "\n";
}
