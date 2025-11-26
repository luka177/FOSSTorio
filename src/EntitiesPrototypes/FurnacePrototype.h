#pragma once
#include <EntitiesPrototypes/CraftingMachinePrototype.h>
#include <EntitiesPrototypes/FurnacePrototype.h>

class FurnacePrototype: public CraftingMachinePrototype {
public:
    FurnacePrototype(sol::table entitiy);
    virtual ~FurnacePrototype() = default;
protected:
    ItemStackIndex result_inventory_size;
    ItemStackIndex source_inventory_size;
    std::string cant_insert_at_source_message_key = "inventory-restriction.cant-be-smelted";
    std::string custom_input_slot_tooltip_key = "";
    // circuit_connector
    // circuit_connector_flipped
    double circuit_wire_max_distance = 0;
    bool draw_copper_wires = true;
    bool draw_circuit_wires = true;
    // std::optional <SignalIDConnector> default_recipe_finished_signal;
    // std::optional <SignalIDConnector> default_working_signal;
};