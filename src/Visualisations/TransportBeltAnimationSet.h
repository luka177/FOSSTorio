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

    // NOTE: You should NEVER pass any direction beside
    // North East South or West, as they are irrelvant for belts
    uint8_t directionToDirectionID(Direction dir) const;

    // NOTE: Only 90 degree turns are valid
    uint8_t cornerDirectionToDirectionID(Direction from, Direction to) const;
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
   // Corners are actually TransportBeltAnimationSetWithCorners and not TransportBeltAnimationSet as per:
   // https://lua-api.factorio.com/latest/types/TransportBeltAnimationSetWithCorners.html
   // but I see no particular benefit of such split considering default values are enforced
   // Obviously directions count should be enforced
   uint8_t east_to_north_index = 5;
   uint8_t north_to_east_index = 6;
   uint8_t west_to_north_index = 7;
   uint8_t north_to_west_index = 8;
   uint8_t south_to_east_index = 9;
   uint8_t east_to_south_index = 10;
   uint8_t south_to_west_index = 11;
   uint8_t west_to_south_index = 12;
};