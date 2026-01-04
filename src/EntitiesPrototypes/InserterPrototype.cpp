#include <EntitiesPrototypes/InserterPrototype.h>

InserterPrototype::InserterPrototype(sol::table entity)
    : EntityWithOwnerPrototype(entity) {
    sol::object obj;

    if(entity["extension_speed"].valid()) {
        extension_speed = entity["extension_speed"].get<double>();
    } else {
        throw std::runtime_error("[InserterPrototype] extension_speed isnt optional, but is missing!\n");
    }

    if(entity["rotation_speed"].valid()) {
        rotation_speed = entity["rotation_speed"].get<double>();
    } else {
        throw std::runtime_error("[InserterPrototype] rotation_speed isnt optional, but is missing!\n");
    }

    starting_distance = entity["circuit_wire_max_distance"].get_or(0.7);

    if(entity["insert_position"].valid()) {
        insert_position = parseVector(entity["insert_position"].get<sol::table>());
    } else {
        throw std::runtime_error("[InserterPrototype] insert_position isnt optional, but is missing!\n");
    }

    if(entity["pickup_position"].valid()) {
        pickup_position = parseVector(entity["pickup_position"].get<sol::table>());
    } else {
        throw std::runtime_error("[InserterPrototype] pickup_position isnt optional, but is missing!\n");
    }

    if(entity["platform_picture"].valid()) {
       platform_picture = Sprite4Way(entity["platform_picture"].get<sol::table>());
    }

    if(entity["platform_frozen"].valid()) {
       platform_frozen = Sprite4Way(entity["platform_frozen"].get<sol::table>());
    }

    if(entity["hand_base_picture"].valid()) {
       hand_base_picture = Sprite(entity["hand_base_picture"].get<sol::table>());
    }

    if(entity["hand_open_picture"].valid()) {
       hand_open_picture = Sprite(entity["hand_open_picture"].get<sol::table>());
    }

    if(entity["hand_closed_picture"].valid()) {
       hand_closed_picture = Sprite(entity["hand_closed_picture"].get<sol::table>());
    }

    if(entity["hand_base_frozen"].valid()) {
       hand_base_frozen = Sprite(entity["hand_base_frozen"].get<sol::table>());
    }

    if(entity["hand_open_frozen"].valid()) {
       hand_open_frozen = Sprite(entity["hand_open_frozen"].get<sol::table>());
    }

    if(entity["hand_closed_frozen"].valid()) {
       hand_closed_frozen = Sprite(entity["hand_closed_frozen"].get<sol::table>());
    }

    if(entity["hand_base_shadow"].valid()) {
       hand_base_shadow = Sprite(entity["hand_base_shadow"].get<sol::table>());
    }

    if(entity["hand_open_shadow"].valid()) {
       hand_open_shadow = Sprite(entity["hand_open_shadow"].get<sol::table>());
    }

    if(entity["hand_closed_shadow"].valid()) {
       hand_closed_shadow = Sprite(entity["hand_closed_shadow"].get<sol::table>());
    }

    bulk = entity["bulk"].get_or(false);
    uses_inserter_stack_size_bonus = entity["uses_inserter_stack_size_bonus"].get_or(true);
    allow_custom_vectors = entity["allow_custom_vectors"].get_or(false);
    allow_burner_leech = entity["allow_burner_leech"].get_or(false);
    draw_held_item = entity["draw_held_item"].get_or(true);
    use_easter_egg = entity["use_easter_egg"].get_or(true);
    grab_less_to_match_belt_stack = entity["grab_less_to_match_belt_stack"].get_or(false);
    wait_for_full_hand = entity["wait_for_full_hand"].get_or(false);
    enter_drop_mode_if_held_stack_spoiled = entity["enter_drop_mode_if_held_stack_spoiled"].get_or(false);

#ifdef SPACEAGE
    max_belt_stack_size = entity["max_belt_stack_size"].get_or(1);
#endif

    filter_count = entity["filter_count"].get_or(0);
    hand_size = entity["hand_size"].get_or(0.75);
    circuit_wire_max_distance = entity["circuit_wire_max_distance"].get_or(0);
    draw_copper_wires = entity["draw_copper_wires"].get_or(true);
    draw_circuit_wires = entity["draw_circuit_wires"].get_or(true);
    draw_inserter_arrow = entity["draw_inserter_arrow"].get_or(true);
    chases_belt_items = entity["chases_belt_items"].get_or(true);
    stack_size_bonus = entity["stack_size_bonus"].get_or(0);

    std::cout << "[InserterPrototype] Created entity prototype " << name << "\n";
}