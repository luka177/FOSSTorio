#include <EntitiesPrototypes/AssemblingMachinePrototype.h>

AssemblingMachinePrototype::AssemblingMachinePrototype(sol::table entity)
    : CraftingMachinePrototype(entity) {
    fixed_recipe = entity["fixed_recipe"].get_or(std::string{});
    gui_title_key = entity["gui_title_key"].get_or(std::string{});
    circuit_wire_max_distance = entity["circuit_wire_max_distance"].get_or(0);
    draw_copper_wires = entity["draw_copper_wires"].get_or(true);
    draw_circuit_wires = entity["draw_circuit_wires"].get_or(true);
    ingredient_count = entity["ingredient_count"].get_or(65535);
    max_item_product_count = entity["max_item_product_count"].get_or(65535);
    fluid_boxes_off_when_no_fluid_recipe = entity["fluid_boxes_off_when_no_fluid_recipe"].get_or(false);
    disabled_when_recipe_not_researched = entity["disabled_when_recipe_not_researched"].get_or(fixed_recipe.empty());

    std::cout << "[AssemblingMachinePrototype] Created entity prototype " << name << "\n";
}
