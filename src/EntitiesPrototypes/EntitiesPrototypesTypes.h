#pragma once

#include <string>
#include <vector>

#include <sol/sol.hpp>

typedef float RealOrientation;

struct MinableProperties {
    double mining_time;
    bool include_in_show_counts;
    bool transfer_entity_health_to_products;
//    EntityPrototype *result;
 //   FluidAmount FluidContent;
//  ParticleID MiningParticle;
  //RequiredFluid RequiredFluids[];
    uint16_t count;
};

enum EntityPrototypeFlags {
    NotRotatable             = 1 << 0,
    PlaceableNeutral         = 1 << 1,
    PlaceablePlayer          = 1 << 2,
    PlaceableEnemy           = 1 << 3,
    PlaceableOffGrid         = 1 << 4,
    PlayerCreation           = 1 << 5,
    BuildingDirection8Way    = 1 << 6,
    FilterDirections         = 1 << 7,
    GetByUnitNumber          = 1 << 8,
    BreathsAir               = 1 << 9,
    NotRepairable            = 1 << 10,
    NotOnMap                 = 1 << 11,
    NotDeconstructable       = 1 << 12,
    NotBlueprintable         = 1 << 13,
    HideAltInfo              = 1 << 14,
    NotFlammable             = 1 << 15,
    NoAutomatedItemRemoval   = 1 << 16,
    NoAutomatedItemInsertion = 1 << 17,
    NoCopyPaste              = 1 << 18,
    NotSelectableInGame      = 1 << 19,
    NotUpgradable            = 1 << 20,
    NotInKillStatistics      = 1 << 21,
    BuildingDirection16Way   = 1 << 22,
    SnapToRailSupportSpot    = 1 << 23,
    NotInMadeIn              = 1 << 24
};

enum EffectTypeLimitation  {
    None                     = 0,
    Speed                    = 1 << 0,
    Productivity             = 1 << 1,
    Consumption              = 1 << 2,
    Pollution                = 1 << 3,
    Quality                  = 1 << 4
};

enum class RenderLayer {
    Zero,
    BackgroundTransitions,
    UnderTiles,
    Decals,
    AboveTiles,
    GroundLayer1,
    GroundLayer2,
    GroundLayer3,
    GroundLayer4,
    GroundLayer5,
    LowerRadiusVisualization,
    RadiusVisualization,
    TransportBeltIntegration,
    Resource,
    BuildingSmoke,
    RailStonePathLower,
    RailStonePath,
    RailTie,
    RailScrew,
    RailMetal,
    Remnants,
    Floor,
    TransportBelt,
    TransportBeltEndings,
    FloorMechanicsUnderCorpse,
    Corpse,
    FloorMechanics,
    Item,
    TransportBeltReader,
    LowerObject,
    TransportBeltCircuitConnector,
    LowerObjectAboveShadow,
    LowerObjectOverlay,
    ObjectUnder,
    Object,
    CargoHatch,
    HigherObjectUnder,
    HigherObjectAbove,
    TrainStopTop,
    ItemInInserterHand,
    AboveInserters,
    Wires,
    UnderElevated,
    ElevatedRailStonePathLower,
    ElevatedRailStonePath,
    ElevatedRailTie,
    ElevatedRailScrew,
    ElevatedRailMetal,
    ElevatedLowerObject,
    ElevatedObject,
    ElevatedHigherObject,
    FluidVisualization,
    WiresAbove,
    EntityInfoIcon,
    EntityInfoIconAbove,
    Explosion,
    Projectile,
    Smoke,
    AirObject,
    AirEntityInfoIcon,
    LightEffect,
    SelectionBox,
    HigherSelectionBox,
    CollisionSelectionBox,
    Arrow,
    Cursor
};

enum RemoveDecoratives {
    False = 0,
    True = 1,
    Automatic = 2
};

struct EffectReceiver {
 //   Effect base_effect;
    bool uses_module_effects;
    bool uses_beacon_effects;
    bool uses_surface_effects;
};

struct MapPosition {
    double x;
    double y;
};

struct BoundingBox {
    MapPosition left_top;
    MapPosition right_bottom;
    RealOrientation orientation;
};

struct IconDrawSpecification {
    std::vector<float> shift;
    float scale;
    float scale_for_many;
    RenderLayer render_layer;
};

struct TilePosition {
    int32_t x;
    int32_t y;
};

struct Color {
    float r;
    float g;
    float b;
    float alpha;
};

EntityPrototypeFlags parseEntityFlags(sol::object obj);
Color parseColor(sol::object obj);
RemoveDecoratives parseRemoveDecoratives(const std::string& s);
BoundingBox parseBoundingBox(sol::object obj);
EffectTypeLimitation effectFromString(const std::string& s);
RenderLayer renderLayerFromString(const std::string& s);
