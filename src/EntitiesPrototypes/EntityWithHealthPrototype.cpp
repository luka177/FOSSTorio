#include <EntitiesPrototypes/EntityWithHealthPrototype.h>

EntityWithHealthPrototype::EntityWithHealthPrototype(sol::table entity)
    :EntityPrototype(entity) {
    max_health = entity["max_health"].get_or(10);
    healing_per_tick = entity["healing_per_tick"].get_or(0);
    repair_speed_modifier = entity["repair_speed_modifier"].get_or(1);
    alert_when_damaged = entity["alert_when_damaged"].get_or(true);
    hide_resistances = entity["hide_resistances"].get_or(true);
    create_ghost_on_death = entity["create_ghost_on_death"].get_or(true);
    random_corpse_variation = entity["random_corpse_variation"].get_or(false);
    integration_patch_render_layer = renderLayerFromString(entity["integration_patch_render_layer"].get_or(std::string("lower-object")));
    overkill_fraction = entity["overkill_fraction"].get_or(0);

    std::cout << "[EntityWithHealthPrototype] Created entity prototype " << name << " max health " << max_health << "\n";
}