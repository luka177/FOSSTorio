#pragma once
#include <Animation/RotatedAnimation.h>
#include <EntitiesPrototypes/EntitiesPrototypesTypes.h>

class TransportBeltAnimationSet {
public:
    TransportBeltAnimationSet(sol::table craftingmachinegraphicsset);
    std::optional<RotatedAnimation> getAnimation() const {
        return animation_set;
    }
protected:
   std::optional<RotatedAnimation> animation_set;
   // TBD: indexes
};