#include <cctype>
#include <iostream>
#include <unordered_map>

#include "ItemPrototypeTypes.h"

static const std::unordered_map<std::string, ItemPrototypeFlags> itemFlagMap = {
    {"draw-logistic-overlay",               DrawLogisticOverlay},
    {"excluded-from-trash-unrequested",     ExcludedFromTrashUnrequested},
    {"excluded-from-character-lift-weight", ExcludedFromCharacterLiftWeight},
    {"always-show",                         AlwaysShow},
    {"hide-from-bonus-gui",                 HideFromBonusGui},
    {"hide-from-fuel-tooltip",              HideFromFuelTooltip},
    {"not-stackable",                       NotStackable},
    {"primary-place-result",                PrimaryPlaceResult},
    {"mod-openable",                        ModOpenable},
    {"only-in-cursor",                      OnlyInCursor},
    {"spawnable",                           Spawnable},
    {"spoil-result",                        SpoilResult},
    {"ignore-spoil-time-modifier",          IgnoreSpoilTimeModifier},
    {"hide-health-bar-in-world",            HideHealthBarInWorld},
    {"hide-spoilage-bar-in-world",          HideSpoilageBarInWorld}
};

ItemPrototypeFlags parseItemFlags(sol::object obj) {
    uint32_t mask = 0;

    if (!obj.valid() || obj.get_type() != sol::type::table)
        return static_cast<ItemPrototypeFlags>(0);

    sol::table tbl = obj.as<sol::table>();

    for (auto& kv : tbl) {
        std::string s = kv.second.as<std::string>();

        auto it = itemFlagMap.find(s);
        if (it != itemFlagMap.end()) {
            mask |= it->second;
        } else {
            std::cerr << "[ItemPrototype] Unknown flag: " << s << "\n";
        }
    }

    return static_cast<ItemPrototypeFlags>(mask);
}

double parseEnergy(const std::string& s) {
    if (s.empty())
        return 0.0;

    std::size_t i = 0;
    while (i < s.size() && (std::isdigit((unsigned char)s[i]) || s[i] == '.'))
        ++i;

    double value = 0.0;
    try {
        value = std::stod(s.substr(0, i));
    } catch (...) {
        return 0.0;
    }

    std::string suffix = s.substr(i);
    double multiplier = 1.0;
    if (!suffix.empty()) {
        switch (suffix[0]) {
            case 'k': case 'K': multiplier = 1e3; break;
            case 'M':           multiplier = 1e6; break;
            case 'G':           multiplier = 1e9; break;
            case 'T':           multiplier = 1e12; break;
            case 'P':           multiplier = 1e15; break;
            default: break;
        }
    }

    return value * multiplier;
}
