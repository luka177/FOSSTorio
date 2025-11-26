#pragma once

#include <string>

struct Vec2 {
    double x;
    double y;
};

class Entity {
public:
    Entity(const std::string& name, const Vec2& position);

    void update(double dt);
    void draw();

    const std::string& getName() const { return name; }
    const Vec2& getPosition() const { return position; }

private:
    std::string name;
    Vec2 position;
};
