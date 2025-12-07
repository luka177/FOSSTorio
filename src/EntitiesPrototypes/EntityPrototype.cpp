#include <EntitiesPrototypes/EntityPrototype.h>

#include <Lua/LuaHelper.h>

EntityPrototype::EntityPrototype(sol::table entity)
    :Prototype(entity) {
    // So far I dont see why would i need to keep path
    icon_size = entity["icon_size"].get_or(64);
    if (entity["icon"].valid()) {
        icon = TextureAtlasSystem::getInstance().requestSprite(resolveLuaPath(entity["icon"].get<std::string>()), 0, 0, 1, 1, icon_size, icon_size);
    } else {
        throw std::runtime_error("[EntityPrototype] icon is needed!\n");
    }

    collision_box = parseBoundingBox(entity["collision_box"]);
    map_generator_bounding_box = parseBoundingBox(entity["map_generator_bounding_box"]);
    selection_box = parseBoundingBox(entity["selection_box"]);
    drawing_box_vertical_extension = entity["drawing_box_vertical_extension"].get_or(0);
    sticker_box = parseBoundingBox(entity["sticker_box"]);
    hit_visualization_box = parseBoundingBox(entity["hit_visualization_box"]);
    flags = parseEntityFlags(entity["flags"]);
    selection_priority = entity["selection_priority"].get_or(50);
    build_grid_size = entity["build_grid_size"].get_or(1);
    remove_decoratives = parseRemoveDecoratives(entity["remove_decoratives"].get_or(std::string("automatic")));
    shooting_cursor_size = entity["shooting_cursor_size"].get_or(1);
    draw_stateless_visualisations_in_ghost = entity["draw_stateless_visualisations_in_ghost"].get_or(false);
    build_base_evolution_requirement = entity["build_base_evolution_requirement"].get_or(0);
    alert_icon_scale = entity["alert_icon_scale"].get_or(1);
    fast_replaceable_group = entity["fast_replaceable_group"].get_or(std::string{});
    protected_from_tile_building = entity["protected_from_tile_building"].get_or(true);
    allow_copy_paste = entity["allow_copy_paste"].get_or(true);
    selectable_in_game = entity["selectable_in_game"].get_or(true);
    //TBD: Base default of collision box
    tile_width = entity["tile_width"].get_or(32);
    tile_height = entity["tile_height"].get_or(32);
    // Sadly it's used 0 times in Factorio, and docs dont mention exexct format'
    //diagonal_tile_grid_size = SOMETHING
    map_color = parseColor(entity["map_color"]);
    friendly_map_color = parseColor(entity["friendly_map_color"]);
    enemy_map_color = parseColor(entity["enemy_map_color"]);
    // TBD: icon_draw_specification
    std::cout << "[EntityPrototype] Created entity prototype " << name << "\n";
}