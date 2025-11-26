#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

#include <sol/sol.hpp>

typedef uint32_t PrototypeID;

class PrototypeBase {
public:
    PrototypeBase(sol::table entities);
    virtual ~PrototypeBase() = default;
    const std::string& getName() const;
    PrototypeID getId() const;
    void setId(PrototypeID id);
protected:
    PrototypeID id;
    //EntityType type;
    const std::string name;
    std::string order;
    //LocalisedString localised_name;
    //LocalisedString localised_description;
    //LocalisedString factoriopedia_description;
    //ItemSubGroupID subgroup;
    bool hidden;
    bool hidden_in_factoriopedia;
    bool parameter;
    //SimulationDefinition factoriopedia_simulation;
};