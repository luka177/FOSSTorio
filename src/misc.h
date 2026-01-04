#pragma once

#include <cstdint>
#include <cmath>
#include <ostream>

constexpr float PI = 3.1415926535f;

template <class T>
struct Vec2T {
    T x{};
    T y{};

    constexpr Vec2T() = default;
    constexpr Vec2T(T x_, T y_) : x(x_), y(y_) {}
    template<class U>
    explicit constexpr Vec2T(const Vec2T<U>& o)
        : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)) {}

    bool operator==(const Vec2T&) const = default;

    friend constexpr Vec2T operator+(Vec2T a, const Vec2T& b) { a += b; return a; }
    friend constexpr Vec2T operator-(Vec2T a, const Vec2T& b) { a -= b; return a; }
    friend constexpr Vec2T operator*(Vec2T a, const Vec2T& b) { a *= b; return a; }
    friend constexpr Vec2T operator/(Vec2T a, const Vec2T& b) { a /= b; return a; }

    constexpr Vec2T& operator+=(const Vec2T& o) { x += o.x; y += o.y; return *this; }
    constexpr Vec2T& operator-=(const Vec2T& o) { x -= o.x; y -= o.y; return *this; }
    constexpr Vec2T& operator*=(const Vec2T& o) { x *= o.x; y *= o.y; return *this; }
    constexpr Vec2T& operator/=(const Vec2T& o) { x /= o.x; y /= o.y; return *this; }

    template<class U>
    constexpr Vec2T& operator+=(const Vec2T<U>& o) {
        x = static_cast<T>(x + o.x);
        y = static_cast<T>(y + o.y);
        return *this;
    }

    friend constexpr Vec2T operator+(Vec2T a, T s) { a += s; return a; }
    friend constexpr Vec2T operator-(Vec2T a, T s) { a -= s; return a; }
    friend constexpr Vec2T operator*(Vec2T a, T s) { a *= s; return a; }
    friend constexpr Vec2T operator/(Vec2T a, T s) { a /= s; return a; }

    friend constexpr Vec2T operator+(T s, Vec2T a) { return a + s; }
    friend constexpr Vec2T operator*(T s, Vec2T a) { return a * s; }

    constexpr Vec2T& operator+=(T s) { x += s; y += s; return *this; }
    constexpr Vec2T& operator-=(T s) { x -= s; y -= s; return *this; }
    constexpr Vec2T& operator*=(T s) { x *= s; y *= s; return *this; }
    constexpr Vec2T& operator/=(T s) { x /= s; y /= s; return *this; }
};

template<class T>
std::ostream& operator<<(std::ostream& os, const Vec2T<T>& v) {
    return os << '(' << v.x << ", " << v.y << ')';
}

template<class A, class B>
using Vec2Common = Vec2T<std::common_type_t<A, B>>;

template<class A, class B>
constexpr auto operator+(Vec2T<A> a, const Vec2T<B>& b) -> Vec2Common<A,B> {
    using R = std::common_type_t<A,B>;
    return { static_cast<R>(a.x) + static_cast<R>(b.x),
             static_cast<R>(a.y) + static_cast<R>(b.y) };
}

template<class A, class B>
constexpr auto operator-(Vec2T<A> a, const Vec2T<B>& b) -> Vec2Common<A,B> {
    using R = std::common_type_t<A,B>;
    return { static_cast<R>(a.x) - static_cast<R>(b.x),
             static_cast<R>(a.y) - static_cast<R>(b.y) };
}

using Vec2  = Vec2T<int64_t>;
using Vec2f = Vec2T<float>;
using Vec2d = Vec2T<double>;

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

inline Vec2 dirToVec(Direction d)
{
    switch (d) {
        case Direction::East:  return {  1,  0 };
        case Direction::West:  return { -1,  0 };
        case Direction::North: return {  0,  1 };
        case Direction::South: return {  0, -1 };
    }
    return {0,0};
}

inline float directionToAngle(Direction d)
{
    switch (d) {
        case Direction::East:  return 0.0f;
        case Direction::North: return 0.5f * PI;
        case Direction::West:  return PI;
        case Direction::South: return 1.5f * PI;
    }
    return 0.0f;
}

inline float directionToCompassAngle(Direction d)
{
    switch (d) {
        case Direction::North: return 0.0f;
        case Direction::East:  return 0.5f * PI;
        case Direction::South: return PI;
        case Direction::West:  return 1.5f * PI;
    }
    return 0.0f;
}

template <class T>
constexpr Vec2T<T> rotateCW(Vec2T<T> v) {
    return { v.y, T{} - v.x };
}

template <class T>
constexpr Vec2T<T> rotateVecToDirection(Vec2T<T> v, Direction d) {
    switch (d) {
        case Direction::North: return v;
        case Direction::East:  return rotateCW(v);
        case Direction::South: return rotateCW(rotateCW(v));
        case Direction::West:  return rotateCW(rotateCW(rotateCW(v)));
        default: return v;
    }
}

template <class TA, class TB>
auto VecToDistance(const Vec2T<TA>& a, const Vec2T<TB>& b) {
    using R = std::common_type_t<TA, TB, double>;
    R dx = R(b.x) - R(a.x);
    R dy = R(b.y) - R(a.y);
    return std::sqrt(dx * dx + dy * dy);
}

template <class TA, class TB>
double VecToNormalizedAngle(const Vec2T<TA>& a, const Vec2T<TB>& b) {
    using R = std::common_type_t<TA, TB, double>;
    R ax = R(a.x), ay = R(a.y);
    R bx = R(b.x), by = R(b.y);

    R dot   = ax * bx + ay * by;
    R cross = ax * by - ay * bx;

    R angle = std::atan2(cross, dot);
    R turns = angle / (R(2) * std::numbers::pi_v<R>);
    if (turns < R(0)) turns += R(1);
    return double(turns);
}

template <class T>
double VecToNormalizedAngle(const Vec2T<T>& v) {
    using R = std::common_type_t<T, double>;
    R x = R(v.x);
    R y = R(v.y);

    R angle = std::atan2(y, x);

    R turns = angle / (R(2) * std::numbers::pi_v<R>);
    if (turns < R(0)) turns += R(1);

    return double(turns);
}
