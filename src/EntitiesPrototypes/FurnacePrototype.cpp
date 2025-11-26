#include <EntitiesPrototypes/FurnacePrototype.h>

FurnacePrototype::FurnacePrototype(sol::table entity)
    : CraftingMachinePrototype(entity) {
    sol::object obj;
    obj = entity["result_inventory_size"];
    if (obj.valid()) {
        result_inventory_size = obj.as<int>();
    } else {
        throw std::runtime_error("[FurnacePrototype] result_inventory_size isnt optional, but is missing!\n");
    }

    obj = entity["source_inventory_size"];
    if (obj.valid()) {
        source_inventory_size = obj.as<int>();
    } else {
        throw std::runtime_error("[FurnacePrototype] source_inventory_size isnt optional, but is missing!\n");
    }

    circuit_wire_max_distance = entity["circuit_wire_max_distance"].get_or(0);
    draw_copper_wires = entity["draw_copper_wires"].get_or(true);
    draw_circuit_wires = entity["draw_circuit_wires"].get_or(true);

    std::cout << "[FurnacePrototype] Created entity prototype " << name << "\n";
}