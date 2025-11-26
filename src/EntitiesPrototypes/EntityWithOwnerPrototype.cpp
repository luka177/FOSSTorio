#include <EntitiesPrototypes/EntityWithOwnerPrototype.h>

EntityWithOwnerPrototype::EntityWithOwnerPrototype(sol::table entity)
    :EntityWithHealthPrototype(entity) {
    is_military_target = entity["is_military_target"].get_or(false);
    allow_run_time_change_of_is_military_target = entity["allow_run_time_change_of_is_military_target"].get_or(false);
    //is_military_target = entity["is_military_target"].get_or(false);
    quality_indicator_scale = entity["quality_indicator_scale"].get_or(1.0);

    std::cout << "[EntityWithOwnerPrototype] Created entity prototype " << name << "\n";
}