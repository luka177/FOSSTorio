#pragma once

#include <string>

#include <Prototype/PrototypeRegister.h>

struct Vec2 {
    double x;
    double y;
};

class Entity {
public:
    Entity(const PrototypeID prototypeId, const Vec2& position);

    void update(double dt);
    void draw();

    const PrototypeID getPrototypeId() const { return prototypeId; }
    const Vec2& getPosition() const { return position; }

private:
    PrototypeID prototypeId;
    Vec2 position;
};
