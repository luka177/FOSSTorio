#pragma once
#include <string>

#include <sol/sol.hpp>

enum class SpritePriority {
    ExtraHighNoScale,
    ExtraHigh,
    High,
    Medium,
    Low,
    VeryLow,
    NoAtlas
};

enum class SpriteFlags : uint64_t {
    NoCrop                 = 1ULL << 0,
    NotCompressed          = 1ULL << 1,
    AlwaysCompressed       = 1ULL << 2,
    Mipmap                 = 1ULL << 3,
    LinearMinification     = 1ULL << 4,
    LinearMagnification    = 1ULL << 5,
    LinearMipLevel         = 1ULL << 6,
    AlphaMask              = 1ULL << 7,
    NoScale                = 1ULL << 8,
    Mask                   = 1ULL << 9,

    // Those expand to multiple flags
    Icon                   = 1ULL << 10,
    GUI                    = 1ULL << 11,
    GUIIcon                = 1ULL << 12,
    Light                  = 1ULL << 13,
    Terrain                = 1ULL << 14,
    TerrainEffectMap       = 1ULL << 15,
    ReflectionEffectMap    = 1ULL << 16,
    Shadow                 = 1ULL << 17,
    Smoke                  = 1ULL << 18,
    Decal                  = 1ULL << 19,
    LowObject              = 1ULL << 20,
    CorpseDecay            = 1ULL << 21,
    TrilinearFiltering     = 1ULL << 22,

    GroupNone              = 1ULL << 23,
    GroupTerrain           = 1ULL << 24,
    GroupShadow            = 1ULL << 25,
    GroupSmoke             = 1ULL << 26,
    GroupDecal             = 1ULL << 27,
    GroupLowObject         = 1ULL << 28,
    GroupGUI               = 1ULL << 29,
    GroupIcon              = 1ULL << 30,
    GroupIconBackground    = 1ULL << 31,
    GroupEffectTexture     = 1ULL << 32
};

inline SpriteFlags operator|(SpriteFlags lhs, SpriteFlags rhs) {
    using underlying = std::underlying_type_t<SpriteFlags>;
    return static_cast<SpriteFlags>(
        static_cast<underlying>(lhs) | static_cast<underlying>(rhs)
    );
}

inline SpriteFlags& operator|=(SpriteFlags& lhs, const SpriteFlags& rhs) {
    lhs = lhs | rhs;
    return lhs;
}

enum class SpriteUsageSurfaceHint {
    Any,
    Nauvis,
    Vulcanus,
    Gleba,
    Fulgora,
    Aqilo,
    Space
};

enum class SpriteUsageHint {
    Any,
    Mining,
    TileArtificial,
    CorpseDecay,
    Enemy,
    Player,
    Train,
    Vehicle,
    Exploision,
    Rail,
    ElevatedRail,
    Air,
    Remnant,
    Decorative
};

SpritePriority parseSpritePriority(const std::string& s);
SpriteFlags parseSpriteSourceFlags(sol::object obj);
SpriteUsageSurfaceHint parseSpriteUsageSurfaceHint(const std::string& s);
SpriteUsageHint parseSpriteUsageHint(const std::string& s);