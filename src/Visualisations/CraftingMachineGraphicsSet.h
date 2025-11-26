#pragma once
#include <Visualisations/WorkingVisualisations.h>
#include <EntitiesPrototypes/EntitiesPrototypesTypes.h>

class CraftingMachineGraphicsSet : public WorkingVisualisations {
public:
    CraftingMachineGraphicsSet(sol::table craftingmachinegraphicsset);
protected:
    //Sprite4Way frozen_patch;
    RenderLayer circuit_connector_layer;
    int8_t circuit_connector_secondary_draw_order = 100;
    float animation_progress = 0.5;
    bool reset_animation_when_frozen = false;
    //WaterReflectionDefinition water_reflection;
};