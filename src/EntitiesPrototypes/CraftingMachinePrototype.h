#pragma once
#include <vector>

#include <EntitiesPrototypes/EntityWithOwnerPrototype.h>
#include <Visualisations/CraftingMachineGraphicsSet.h>
typedef uint16_t ItemStackIndex;

enum Mirroring{
    Horizontal,
    Vertical,
    DiagonlPos,
    DiagonalNeg,
    MirroringNone
};

class CraftingMachinePrototype: public EntityWithOwnerPrototype {
public:
    CraftingMachinePrototype(sol::table entities);
    virtual ~CraftingMachinePrototype() = default;
protected:
    bool quality_affects_energy_usage = false;
    //Energy energy_usage;
    double crafting_speed;
    // RecipeCategoryID crafting_categories[];
    //EnergySource energy_source;
    // FluidBox fluid_boxes[];
    //std::optional<EffectReceiver> effect_receiver;
    ItemStackIndex module_slots = 0;
    bool quality_affects_module_slots = false;
    EffectTypeLimitation allowed_effects = None;
    //ModuleCategoryID allowed_module_categories[];
    bool show_recipe_icon = true;
    bool return_ingredients_on_change = true;
    bool draw_entity_info_icon_background = true;
    bool match_animation_speed_to_activity = true;
    bool show_recipe_icon_on_map = true;
    bool fast_transfer_modules_into_module_slots_only = false;
    bool ignore_output_full = false;
    std::optional<CraftingMachineGraphicsSet> graphics_set;
    std::optional<CraftingMachineGraphicsSet> graphics_set_flipped;
    // std::optional<PerceivedPerformance> perceived_performance;
    // std::optional<ProductionHealthEffect> production_health_effect;
    // std::optional<ItemStackIndex> trash_inventory_size;
    //std::vector<int32_t> vector_to_place_result;
    Mirroring forced_symmetry = MirroringNone;
    // std::map<QuailityID, double> crafting_speed_quality_multiplier;
    // std::map<QuailityID, double> module_slots_quality_bonus;
    // std::map<QuailityID, double> energy_usage_quality_multiplier;
};