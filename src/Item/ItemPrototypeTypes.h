#pragma once
#include <string>

#include <sol/sol.hpp>

enum ItemPrototypeFlags {
    DrawLogisticOverlay             = 1 << 0,
    ExcludedFromTrashUnrequested    = 1 << 1,
    ExcludedFromCharacterLiftWeight = 1 << 2,
    AlwaysShow                      = 1 << 3,
    HideFromBonusGui                = 1 << 4,
    HideFromFuelTooltip             = 1 << 5,
    NotStackable                    = 1 << 6,
    PrimaryPlaceResult              = 1 << 7,
    ModOpenable                     = 1 << 8,
    OnlyInCursor                    = 1 << 9,
    Spawnable                       = 1 << 10,
    SpoilResult                     = 1 << 11,
    IgnoreSpoilTimeModifier         = 1 << 12,
    HideHealthBarInWorld            = 1 << 13,
    HideSpoilageBarInWorld          = 1 << 14
};

ItemPrototypeFlags parseItemFlags(sol::object obj);
// Parses strings in the "<number>[unit-prefix]<unit>" form Factorio uses for Energy, e.g. "1MJ" -> 1000000
double parseEnergy(const std::string& s);
