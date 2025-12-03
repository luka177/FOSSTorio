#include <EntitiesPrototypes/TransportBeltConnectablePrototype.h>

TransportBeltConnectablePrototype::TransportBeltConnectablePrototype(sol::table entity)
    : EntityWithOwnerPrototype(entity) {
    sol::object obj;

    obj = entity["belt_animation_set"];
    if (obj.valid()) {
        belt_animation_set = TransportBeltAnimationSet(entity["belt_animation_set"].get<sol::table>());
    }

    speed = entity["speed"].get_or(1);
    animation_speed_coefficient = entity["speed"].get_or(1);

    std::cout << "[TransportBeltConnectablePrototype] Created entity prototype " << name << "\n";
}