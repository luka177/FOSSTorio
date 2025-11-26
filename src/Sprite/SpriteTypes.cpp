#include <unordered_map>
#include <iostream>

#include <Sprite/SpriteTypes.h>

#include <sol/sol.hpp>

static const std::unordered_map<std::string, SpritePriority> SpritePriorityMap = {
    { "extra-high-no-scale",     SpritePriority::ExtraHighNoScale },
    { "extra-high",              SpritePriority::ExtraHigh },
    { "high",                    SpritePriority::High },
    { "medium",                  SpritePriority::Medium },
    { "low",                     SpritePriority::Low },
    { "very-low",                SpritePriority::VeryLow },
    { "no-atlas",                SpritePriority::NoAtlas }
};

static const std::unordered_map<std::string, SpriteFlags> SpriteFlagsMap = {
    { "no-crop",                 SpriteFlags::NoCrop },
    { "not-compressed",          SpriteFlags::NotCompressed },
    { "always-compressed",       SpriteFlags::AlwaysCompressed },
    { "mipmap",                  SpriteFlags::Mipmap },
    { "linear-minification",     SpriteFlags::LinearMinification },
    { "linear-magnification",    SpriteFlags::LinearMagnification },
    { "linear-mip-level",        SpriteFlags::LinearMipLevel },
    { "alpha-mask",              SpriteFlags::AlphaMask },
    { "no-scale",                SpriteFlags::NoScale },
    { "mask",                    SpriteFlags::Mask },
    { "shadow",                  SpriteFlags::Shadow },
    { "smoke",                   SpriteFlags::Smoke },
    { "decal",                   SpriteFlags::Decal },
    { "low-object",              SpriteFlags::LowObject },
    { "corpse-decay",            SpriteFlags::CorpseDecay },
    { "trilinear-filtering",     SpriteFlags::TrilinearFiltering },

    // Those expand to multiple flags
    { "icon",                    SpriteFlags::Icon },
    { "gui",                     SpriteFlags::GUI },
    { "gui-icon",                SpriteFlags::GUIIcon },
    { "light",                   SpriteFlags::Light },
    { "terrain",                 SpriteFlags::Terrain },
    { "terrain-effect-map",      SpriteFlags::TerrainEffectMap },
    { "reflection-effect-map",   SpriteFlags::ReflectionEffectMap },

    { "group=none",              SpriteFlags::GroupNone },
    { "group=terrain",           SpriteFlags::GroupTerrain },
    { "group=shadow",            SpriteFlags::GroupShadow },
    { "group=smoke",             SpriteFlags::GroupSmoke },
    { "group=decal",             SpriteFlags::GroupDecal },
    { "group=low-object",        SpriteFlags::GroupLowObject },
    { "group=gui",               SpriteFlags::GroupGUI },
    { "group=icon",              SpriteFlags::GroupIcon },
    { "group=icon-background",   SpriteFlags::GroupIconBackground },
    { "group=effect-texture",    SpriteFlags::GroupEffectTexture }
};

static const std::unordered_map<std::string, SpriteUsageSurfaceHint> SpriteUsageSurfaceHintMap = {
    { "any",                 SpriteUsageSurfaceHint::Any },
    { "nauvis",              SpriteUsageSurfaceHint::Nauvis },
    { "vulcanus",            SpriteUsageSurfaceHint::Vulcanus },
    { "gleba",               SpriteUsageSurfaceHint::Gleba },
    { "fulgora",             SpriteUsageSurfaceHint::Fulgora },
    { "aqilo",               SpriteUsageSurfaceHint::Aqilo },
    { "space",               SpriteUsageSurfaceHint::Space }
};

static const std::unordered_map<std::string, SpriteUsageHint> SpriteUsageHintMap = {
    { "any",                 SpriteUsageHint::Any },
    { "mining",              SpriteUsageHint::Mining },
    { "tile-artificial",     SpriteUsageHint::TileArtificial },
    { "corpse-decay",        SpriteUsageHint::CorpseDecay },
    { "enemy",               SpriteUsageHint::Enemy },
    { "player",              SpriteUsageHint::Player },
    { "train",               SpriteUsageHint::Train },
    { "vehicle",             SpriteUsageHint::Vehicle },
    { "explosion",           SpriteUsageHint::Exploision },
    { "rail",                SpriteUsageHint::Rail },
    { "elevated-rail",       SpriteUsageHint::ElevatedRail },
    { "air",                 SpriteUsageHint::Air },
    { "remnant",             SpriteUsageHint::Remnant },
    { "decorative",          SpriteUsageHint::Decorative }
};

SpritePriority parseSpritePriority(const std::string& s) {
    auto it = SpritePriorityMap.find(s);
    if (it != SpritePriorityMap.end())
        return it->second;

    return SpritePriority::Medium;
}

SpriteFlags parseSpriteSourceFlags(sol::object obj) {
    SpriteFlags mask = static_cast<SpriteFlags>(0);

    if (!obj.valid() || obj.get_type() != sol::type::table)
        return static_cast<SpriteFlags>(0); // default

    sol::table tbl = obj.as<sol::table>();

    for (auto& kv : tbl) {
        std::string s = kv.second.as<std::string>();

        auto it = SpriteFlagsMap.find(s);
        if (it != SpriteFlagsMap.end()) {
            mask |= it->second;
        } else {
            std::cerr << "[EntityPrototype] Unknown flag: " << s << "\n";
        }
    }

    return static_cast<SpriteFlags>(mask);
}

SpriteUsageSurfaceHint parseSpriteUsageSurfaceHint(const std::string& s) {
    auto it = SpriteUsageSurfaceHintMap.find(s);
    if (it != SpriteUsageSurfaceHintMap.end())
        return it->second;

    return SpriteUsageSurfaceHint::Any;
}

SpriteUsageHint parseSpriteUsageHint(const std::string& s) {
    auto it = SpriteUsageHintMap.find(s);
    if (it != SpriteUsageHintMap.end())
        return it->second;

    return SpriteUsageHint::Any;
}