#pragma once

#include <cstdint>
#include <ostream>

struct Vec2 {
    int64_t x;
    int64_t y;

    bool operator==(const Vec2&) const = default;
};

struct Vec2d {
    double x;
    double y;
};

enum class Direction {
    North = 0,
    NorthNorthEast,
    NorthEast,
    EastNorthEast,
    East,
    EastSouthEast,
    SouthEast,
    SouthSouthEast,
    South,
    SouthSouthWest,
    SouthWest,
    WestSouthWest,
    West,
    WestNorthWest,
    NorthWest,
    NorthNorthWest
};

inline std::ostream& operator<<(std::ostream& os, Direction d) {
    switch (d) {
        case Direction::North:          return os << "N";
        case Direction::NorthNorthEast: return os << "NNE";
        case Direction::NorthEast:      return os << "NE";
        case Direction::EastNorthEast:  return os << "ENE";
        case Direction::East:           return os << "E";
        case Direction::EastSouthEast:  return os << "ESE";
        case Direction::SouthEast:      return os << "SE";
        case Direction::SouthSouthEast: return os << "SSE";
        case Direction::South:          return os << "S";
        case Direction::SouthSouthWest: return os << "SSW";
        case Direction::SouthWest:      return os << "SW";
        case Direction::WestSouthWest:  return os << "WSW";
        case Direction::West:           return os << "W";
        case Direction::WestNorthWest:  return os << "WNW";
        case Direction::NorthWest:      return os << "NW";
        case Direction::NorthNorthWest: return os << "NNW";
    }
    return os << '?';
}