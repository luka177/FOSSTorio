#pragma once
#include <vector>

#include <EntitiesPrototypes/EntityWithHealthPrototype.h>

class EntityWithOwnerPrototype: public EntityWithHealthPrototype {
public:
    EntityWithOwnerPrototype(sol::table entities);
    virtual ~EntityWithOwnerPrototype() = default;
protected:
    bool is_military_target = false;
    bool allow_run_time_change_of_is_military_target = false;
    std::vector<int32_t> quality_indicator_shift;
    double quality_indicator_scale = 1.0;
};