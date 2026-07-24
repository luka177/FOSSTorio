#pragma once
#include <cstdint>
#include <string>

#include <EntitiesPrototypes/CraftingMachinePrototype.h>

class AssemblingMachinePrototype: public CraftingMachinePrototype {
public:
    AssemblingMachinePrototype(sol::table entities);
    virtual ~AssemblingMachinePrototype() = default;
protected:
    std::string fixed_recipe;
    //QualityID fixed_quality;
    std::string gui_title_key;
    double circuit_wire_max_distance = 0;
    bool draw_copper_wires = true;
    bool draw_circuit_wires = true;
    //std::optional <SignalIDConnector> default_recipe_finished_signal;
    //std::optional <SignalIDConnector> default_working_signal;
    uint16_t ingredient_count = 65535;
    uint16_t max_item_product_count = 65535;
    //circuit_connector TBD
    //circuit_connector_flipped TBD
    bool fluid_boxes_off_when_no_fluid_recipe = false;
    bool disabled_when_recipe_not_researched;
};
