#pragma once
#include <cstdint>
#include <optional>
#include <string>

#include <Atlas/TextureAtlasSystem.h>
#include <Sprite/SpriteTypes.h>
#include <Prototype/Prototype.h>
#include "ItemPrototypeTypes.h"

#include <sol/sol.hpp>

class ItemPrototype: public Prototype {
public:
    ItemPrototype(sol::table entities);
    virtual ~ItemPrototype() = default;
    const std::optional<TextureId> getIcon() const {
        return icon;
    };
    uint32_t getStackSize() const {
        return stack_size;
    };
protected:
    uint32_t stack_size;
    std::optional<TextureId> icon;
    SpriteSizeType icon_size = 64;
    //std::vector<IconData> icons;
    //std::optional<TextureId> dark_background_icon;
    //SpriteSizeType dark_background_icon_size = 64;
    std::string place_result;
    //EquipmentID place_as_equipment_result;
    std::string fuel_category;
    std::string burnt_result;
    std::string spoil_result;
    //QualityID spoil_quality_min;
    //QualityID spoil_quality_max;
    int8_t spoil_quality_change = 0;
    std::string plant_result;
    //PlaceAsTile place_as_tile;
    //SpriteVariations pictures;
    ItemPrototypeFlags flags;
    uint32_t spoil_ticks = 0;
    double fuel_value = 0;
    double fuel_acceleration_multiplier = 1.0;
    double fuel_top_speed_multiplier = 1.0;
    double fuel_emissions_multiplier = 1.0;
    //double fuel_acceleration_multiplier_quality_bonus;
    //double fuel_top_speed_multiplier_quality_bonus;
    double weight = 0;
    double ingredient_to_weight_coefficient = 0.5;
    bool space_platform_request_priority = false;
    //Color fuel_glow_color;
    //Sound open_sound;
    //Sound close_sound;
    //Sound pick_sound;
    //Sound drop_sound;
    //Sound inventory_move_sound;
    std::string default_import_location = "nauvis";
    //ColorHintSpecification color_hint;
    bool has_random_tint = true;
    //SpoilToTriggerResult spoil_to_trigger_result;
    //Trigger destroyed_by_dropping_trigger;
    //std::vector<ItemProductPrototype> rocket_launch_products;
    std::string send_to_orbit_mode = "not-sendable";
    bool moved_to_hub_when_building = false;
    //Color random_tint_color;
    uint8_t spoil_level = 0;
    bool auto_recycle = true;
    bool lab_ignores_spoil_percent = false;
};
