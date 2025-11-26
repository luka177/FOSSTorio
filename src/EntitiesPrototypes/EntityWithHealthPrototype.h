#pragma once
#include <EntitiesPrototypes/EntityPrototype.h>

class EntityWithHealthPrototype: public EntityPrototype {
public:
    EntityWithHealthPrototype(sol::table entities);
    virtual ~EntityWithHealthPrototype() = default;
protected:
    float max_health = 10;
    float healing_per_tick = 0;
    float repair_speed_modifier = 1;
    //ExplosionDefinition dying_explosion[];
    //std::optional<TriggerEffect> dying_trigger_effect;
    //std::optional<TriggerEffect> damaged_trigger_effect;
    //LootItem loot[];
    //Resistance resistances[];
    //AttackReactionItem attack_reaction[];
    //std::optional<Sound> repair_sound;
    bool alert_when_damaged = true;
    bool hide_resistances = true;
    bool create_ghost_on_death = true;
    bool random_corpse_variation = false;
    RenderLayer integration_patch_render_layer;
    //EntityID corpse[];
    //std::optional <Sprite4Way> integration_patch;
    float overkill_fraction;
};