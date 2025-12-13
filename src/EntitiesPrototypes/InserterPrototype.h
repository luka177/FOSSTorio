#pragma once
#include <vector>

#include <EntitiesPrototypes/EntityWithOwnerPrototype.h>
#include <Sprite/Sprite.h>
#include <Sprite/Sprite4Way.h>
#include <misc.h>

class InserterPrototype: public EntityWithOwnerPrototype {
public:
    InserterPrototype(sol::table entities);
    virtual ~InserterPrototype() = default;

protected:
    double extension_speed;
    double rotation_speed;
    double starting_distance = 0.7;
    struct Vec2d insert_position;
    struct Vec2d pickup_position;
    std::optional<Sprite4Way> platform_picture;
    std::optional<Sprite4Way> platform_frozen;
    std::optional<Sprite> hand_base_picture;
    std::optional<Sprite> hand_open_picture;
    std::optional<Sprite> hand_closed_picture;
    std::optional<Sprite> hand_base_frozen;
    std::optional<Sprite> hand_open_frozen;
    std::optional<Sprite> hand_closed_frozen;
    std::optional<Sprite> hand_base_shadow;
    std::optional<Sprite> hand_open_shadow;
    std::optional<Sprite> hand_closed_shadow;
    //EnergySource energy_source;
    //Energy energy_per_movement = 0;
    //Energy energy_per_rotation = 0;
    bool bulk = false;
    bool uses_inserter_stack_size_bonus = true;
    bool allow_custom_vectors = true;
    bool allow_burner_leech = false;
    bool draw_held_item = true;
    // Fish for life, fish over factory
    bool use_easter_egg = true;
    bool grab_less_to_match_belt_stack = false;
    bool wait_for_full_hand = false;
    bool enter_drop_mode_if_held_stack_spoiled = false;
#ifdef SPACEAGE
    uint8_t max_belt_stack_size = 1;
#endif
    uint8_t filter_count = 0;
    double hand_size = 0.75;
    double circuit_wire_max_distance = 0;
    bool draw_copper_wires = true;
    bool draw_circuit_wires = true;
    //std::optional <SignalIDConnector> default_stack_control_input_signal;
    bool draw_inserter_arrow = true;
    bool chases_belt_items = true;
    uint8_t stack_size_bonus = 0;
    //circuit_connector TBD
};