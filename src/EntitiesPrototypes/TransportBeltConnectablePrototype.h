#pragma once
#include <vector>

#include <EntitiesPrototypes/EntityWithOwnerPrototype.h>
#include <Visualisations/TransportBeltAnimationSet.h>

class TransportBeltConnectablePrototype: public EntityWithOwnerPrototype {
public:
    TransportBeltConnectablePrototype(sol::table entities);
    virtual ~TransportBeltConnectablePrototype() = default;
    const std::optional<TransportBeltAnimationSet> get_animation_set() const {
        return belt_animation_set;
    };

protected:
    std::optional<TransportBeltAnimationSet> belt_animation_set;
    double speed = 1;
    double animation_speed_coefficient = 1;
};