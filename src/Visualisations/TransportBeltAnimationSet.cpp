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
        case Direction::North:
            return north_index;
    }
    throw std::logic_error("[TransportBeltAnimationSet] invalid direction");
}

uint8_t TransportBeltAnimationSet::cornerDirectionToDirectionID(Direction from, Direction to) const {
    switch(from) {
        case Direction::East:
            switch(to) {
                case Direction::North:
                    return east_to_north_index;
                case Direction::South:
                    return east_to_south_index;
            }
            throw std::logic_error("[TransportBeltAnimationSet] from east can only be to north or south");
        case Direction::West:
            switch(to) {
                case Direction::North:
                    return west_to_north_index;
                case Direction::South:
                    return west_to_south_index;
            }
            throw std::logic_error("[TransportBeltAnimationSet] from west can only be to north or south");
        case Direction::South:
            switch(to) {
                case Direction::East:
                    return south_to_east_index;
                case Direction::West:
                    return south_to_west_index;
            }
            throw std::logic_error("[TransportBeltAnimationSet] from south can only be to east or west");
        case Direction::North:
            switch(to) {
                case Direction::East:
                    return north_to_east_index;
                case Direction::West:
                    return north_to_west_index;
            }
            throw std::logic_error("[TransportBeltAnimationSet] from north can only be to east or west");
    }
    throw std::logic_error("[TransportBeltAnimationSet] invalid from");
}