#include <Visualisations/TransportBeltAnimationSet.h>

TransportBeltAnimationSet::TransportBeltAnimationSet(sol::table transportbeltanimationset) {
    if(transportbeltanimationset["animation_set"].valid()) {
        std::cout << "[TransportBeltAnimationSet] Valid TransportBeltAnimationSet\n";
        animation_set = RotatedAnimation(transportbeltanimationset["animation_set"].get<sol::table>());
    }
};

uint8_t TransportBeltAnimationSet::directionToDirectionID(Direction dir) const {
    switch(dir) {
        case Direction::East:
            return east_index;
        case Direction::West:
            return west_index;
        case Direction::South:
            return south_index;
        default:
            return north_index;
    }
}