#include "EntitiesPrototypesTypes.h"
#include <unordered_map>
#include <iostream>

static const std::unordered_map<std::string, EntityPrototypeFlags> flagMap = {
    {"not-rotatable",            NotRotatable},
    {"placeable-neutral",        PlaceableNeutral},
    {"placeable-player",         PlaceablePlayer},
    {"placeable-enemy",          PlaceableEnemy},
    {"placeable-off-grid",       PlaceableOffGrid},
    {"player-creation",          PlayerCreation},
    {"building-direction-8-way", BuildingDirection8Way},
    {"filter-directions",        FilterDirections},
    {"get-by-unit-number",       GetByUnitNumber},
    {"breaths-air",              BreathsAir},
    {"not-repairable",           NotRepairable},
    {"not-on-map",               NotOnMap},
    {"not-deconstructable",      NotDeconstructable},
    {"not-blueprintable",        NotBlueprintable},
    {"hide-alt-info",            HideAltInfo},
    {"not-flammable",            NotFlammable},
    {"no-automated-item-removal",NoAutomatedItemRemoval},
    {"no-automated-item-insertion", NoAutomatedItemInsertion},
    {"no-copy-paste",            NoCopyPaste},
    {"not-selectable-in-game",   NotSelectableInGame},
    {"not-upgradable",           NotUpgradable},
    {"not-in-kill-statistics",   NotInKillStatistics},
    {"building-direction-16-way",BuildingDirection16Way},
    {"snap-to-rail-support-spot",SnapToRailSupportSpot},
    {"not-in-made-in",           NotInMadeIn}
};

static const std::unordered_map<std::string, RemoveDecoratives> RemoveDecorativesFlagMap = {
    {"false",       False},
    {"true",        True},
    {"automatic",   Automatic}
};

EntityPrototypeFlags parseEntityFlags(sol::object obj) {
    uint32_t mask = 0;

    if (!obj.valid() || obj.get_type() != sol::type::table)
        return static_cast<EntityPrototypeFlags>(0);

    sol::table tbl = obj.as<sol::table>();

    for (auto& kv : tbl) {
        std::string s = kv.second.as<std::string>();

        auto it = flagMap.find(s);
        if (it != flagMap.end()) {
            mask |= it->second;
        } else {
            std::cerr << "[EntityPrototype] Unknown flag: " << s << "\n";
        }
    }

    return static_cast<EntityPrototypeFlags>(mask);
}

Color parseColor(sol::object obj) {
    Color color{1.f, 1.f, 1.f, 1.f};

    if (!obj.valid() || obj.get_type() != sol::type::table)
        return color;

    sol::table tbl = obj.as<sol::table>();

    sol::object or_ = tbl["r"];
    sol::object og_ = tbl["g"];
    sol::object ob_ = tbl["b"];
    sol::object oa_ = tbl["a"];

    if (or_.valid()) color.r = or_.as<double>();
    if (og_.valid()) color.g = og_.as<double>();
    if (ob_.valid()) color.b = ob_.as<double>();
    if (oa_.valid()) color.alpha = oa_.as<double>();

    sol::object i1 = tbl[1];
    sol::object i2 = tbl[2];
    sol::object i3 = tbl[3];
    sol::object i4 = tbl[4];

    if (i1.valid()) color.r = i1.as<double>();
    if (i2.valid()) color.g = i2.as<double>();
    if (i3.valid()) color.b = i3.as<double>();
    if (i4.valid()) color.alpha = i4.as<double>();

    return color;
}

RemoveDecoratives parseRemoveDecoratives(const std::string& s) {
    auto it = RemoveDecorativesFlagMap.find(s);
    if (it != RemoveDecorativesFlagMap.end())
        return it->second;

    return RemoveDecoratives::Automatic;
}

BoundingBox parseBoundingBox(sol::object obj) {
    BoundingBox box{
        {0.0, 0.0},
        {0.0, 0.0},
        0.0f
    };

    if (!obj.valid() || obj.get_type() != sol::type::table) {
        return box;
    }

    sol::table tbl = obj.as<sol::table>();

    sol::object first  = tbl[1];
    sol::object second = tbl[2];

    if (!first.valid() || !second.valid()) {
        throw std::runtime_error("collision_box must be {{x1,y1},{x2,y2}}");
    }

    sol::table lt = first.as<sol::table>();
    sol::table rb = second.as<sol::table>();

    box.left_top.x      = lt[1].get_or(0.0);
    box.left_top.y      = lt[2].get_or(0.0);
    box.right_bottom.x  = rb[1].get_or(0.0);
    box.right_bottom.y  = rb[2].get_or(0.0);

    return box;
}

EffectTypeLimitation effectFromString(const std::string& s) {
    if (s == "speed")         return EffectTypeLimitation::Speed;
    if (s == "productivity")  return EffectTypeLimitation::Productivity;
    if (s == "consumption")   return EffectTypeLimitation::Consumption;
    if (s == "pollution")     return EffectTypeLimitation::Pollution;
    if (s == "quality")       return EffectTypeLimitation::Quality;
    return EffectTypeLimitation::None;
}

RenderLayer renderLayerFromString(const std::string& s) {
    static const std::unordered_map<std::string, RenderLayer> lookup = {
        {"zero", RenderLayer::Zero},
        {"background-transitions", RenderLayer::BackgroundTransitions},
        {"under-tiles", RenderLayer::UnderTiles},
        {"decals", RenderLayer::Decals},
        {"above-tiles", RenderLayer::AboveTiles},
        {"ground-layer-1", RenderLayer::GroundLayer1},
        {"ground-layer-2", RenderLayer::GroundLayer2},
        {"ground-layer-3", RenderLayer::GroundLayer3},
        {"ground-layer-4", RenderLayer::GroundLayer4},
        {"ground-layer-5", RenderLayer::GroundLayer5},
        {"lower-radius-visualization", RenderLayer::LowerRadiusVisualization},
        {"radius-visualization", RenderLayer::RadiusVisualization},
        {"transport-belt-integration", RenderLayer::TransportBeltIntegration},
        {"resource", RenderLayer::Resource},
        {"building-smoke", RenderLayer::BuildingSmoke},
        {"rail-stone-path-lower", RenderLayer::RailStonePathLower},
        {"rail-stone-path", RenderLayer::RailStonePath},
        {"rail-tie", RenderLayer::RailTie},
        {"rail-screw", RenderLayer::RailScrew},
        {"rail-metal", RenderLayer::RailMetal},
        {"remnants", RenderLayer::Remnants},
        {"floor", RenderLayer::Floor},
        {"transport-belt", RenderLayer::TransportBelt},
        {"transport-belt-endings", RenderLayer::TransportBeltEndings},
        {"floor-mechanics-under-corpse", RenderLayer::FloorMechanicsUnderCorpse},
        {"corpse", RenderLayer::Corpse},
        {"floor-mechanics", RenderLayer::FloorMechanics},
        {"item", RenderLayer::Item},
        {"transport-belt-reader", RenderLayer::TransportBeltReader},
        {"lower-object", RenderLayer::LowerObject},
        {"transport-belt-circuit-connector", RenderLayer::TransportBeltCircuitConnector},
        {"lower-object-above-shadow", RenderLayer::LowerObjectAboveShadow},
        {"lower-object-overlay", RenderLayer::LowerObjectOverlay},
        {"object-under", RenderLayer::ObjectUnder},
        {"object", RenderLayer::Object},
        {"cargo-hatch", RenderLayer::CargoHatch},
        {"higher-object-under", RenderLayer::HigherObjectUnder},
        {"higher-object-above", RenderLayer::HigherObjectAbove},
        {"train-stop-top", RenderLayer::TrainStopTop},
        {"item-in-inserter-hand", RenderLayer::ItemInInserterHand},
        {"above-inserters", RenderLayer::AboveInserters},
        {"wires", RenderLayer::Wires},
        {"under-elevated", RenderLayer::UnderElevated},
        {"elevated-rail-stone-path-lower", RenderLayer::ElevatedRailStonePathLower},
        {"elevated-rail-stone-path", RenderLayer::ElevatedRailStonePath},
        {"elevated-rail-tie", RenderLayer::ElevatedRailTie},
        {"elevated-rail-screw", RenderLayer::ElevatedRailScrew},
        {"elevated-rail-metal", RenderLayer::ElevatedRailMetal},
        {"elevated-lower-object", RenderLayer::ElevatedLowerObject},
        {"elevated-object", RenderLayer::ElevatedObject},
        {"elevated-higher-object", RenderLayer::ElevatedHigherObject},
        {"fluid-visualization", RenderLayer::FluidVisualization},
        {"wires-above", RenderLayer::WiresAbove},
        {"entity-info-icon", RenderLayer::EntityInfoIcon},
        {"entity-info-icon-above", RenderLayer::EntityInfoIconAbove},
        {"explosion", RenderLayer::Explosion},
        {"projectile", RenderLayer::Projectile},
        {"smoke", RenderLayer::Smoke},
        {"air-object", RenderLayer::AirObject},
        {"air-entity-info-icon", RenderLayer::AirEntityInfoIcon},
        {"light-effect", RenderLayer::LightEffect},
        {"selection-box", RenderLayer::SelectionBox},
        {"higher-selection-box", RenderLayer::HigherSelectionBox},
        {"collision-selection-box", RenderLayer::CollisionSelectionBox},
        {"arrow", RenderLayer::Arrow},
        {"cursor", RenderLayer::Cursor}
    };

    auto it = lookup.find(s);
    if (it != lookup.end())
        return it->second;

    return RenderLayer::LowerObject;
}