#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <bits/stdc++.h>

#include <Prototype/Prototype.h>
#include "EntitiesPrototypesTypes.h"

#include <sol/sol.hpp>

class EntityPrototype: public Prototype {
public:
    EntityPrototype(sol::table entities);
    virtual ~EntityPrototype() = default;
protected:
    //AtlasTexture icon;
    uint16_t icon_size;
    BoundingBox collision_box;
    //CollisionMaskConnector collision_mask
    BoundingBox map_generator_bounding_box;
    BoundingBox selection_box;
    double drawing_box_vertical_extension;
    BoundingBox sticker_box;
    BoundingBox hit_visualization_box;
    //TriggerTargetMask trigger_target_mask;
    EntityPrototypeFlags flags;
    //TileBuildabilityRule tile_buildability_rules[];
    struct MinableProperties minable;
#ifdef SPACEAGE
    SurfaceCondition surface_conditions[];
#endif
    //EntityID deconstruction_alternative;
    uint8_t selection_priority;
    uint8_t build_grid_size;
    RemoveDecoratives remove_decoratives;
    //std::map<AirbornePollutantID, double> emissions_per_second;
    double shooting_cursor_size;
    //CreateTrivialSmokeEffectItem created_smoke;
    //WorkingSound working_sound;
    //Trigger created_effect;
    //Sound build_sound;
    //Sound mined_sound;
    //Sound mining_sound;
    //Sound rotated_sound;
    //ImpactCategory impact_category;
    //Sound open_sound;
    //Sound close_sound;
    //Sprite placeable_position_visualization;
    //RadiusVisualisationSpecification radius_visualisation_specification
    //StatelessVisualisation stateless_visualisation
    bool draw_stateless_visualisations_in_ghost;
    double build_base_evolution_requirement;
    // vector<TBD> alert_icon_shift;
    float alert_icon_scale;
    std::string fast_replaceable_group;
    //EntityID next_upgrade;
    bool protected_from_tile_building;
#ifdef SPACEAGE
    Energy heating_energy;
#endif
    bool allow_copy_paste;
    bool selectable_in_game;
    // ItemToPlace placeable_by[];
    // EntityID remains_when_mined[];
    // EntityID additional_pastable_entities[];
    int32_t tile_width;
    int32_t tile_height;
    TilePosition diagonal_tile_grid_size;
    // AutoplaceSpecification autoplace;
    Color map_color;
    Color friendly_map_color;
    Color enemy_map_color;
    // WaterReflectionDefinition water_reflection;
    // EntityID ambient_sounds_group;
    // WorldAmbientSoundDefinition ambient_sounds[];
    IconDrawSpecification icon_draw_specification;
    // IconSequencePositioning icons_positioning[];
};
