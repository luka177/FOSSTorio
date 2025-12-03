#pragma once
#include <Animation/RotatedAnimation.h>
#include <EntitiesPrototypes/EntitiesPrototypesTypes.h>
#include <misc.h>

class TransportBeltAnimationSet {
public:
    TransportBeltAnimationSet(sol::table craftingmachinegraphicsset);
    std::optional<RotatedAnimation> getAnimation() const {
        return animation_set;
    }
    uint8_t directionToDirectionID(Direction dir) const;
protected:
   std::optional<RotatedAnimation> animation_set;
   uint8_t east_index = 1;
   uint8_t west_index = 2;
   uint8_t north_index = 3;
   uint8_t south_index = 4;
   uint8_t starting_south_index = 13;
   uint8_t ending_south_index = 14;
   uint8_t starting_west_index = 15;
   uint8_t ending_west_index = 16;
   uint8_t starting_north_index = 17;
   uint8_t ending_north_index = 18;
   uint8_t starting_east_index = 19;
   uint8_t ending_east_index = 20;
};