#pragma once

#include <string>

#include <Prototype/PrototypeRegister.h>

typedef uint32_t EntityID;

struct Vec2 {
    int64_t x;
    int64_t y;
};

class Entity {
public:
    Entity(const PrototypeID prototypeId, const Vec2& position);

    void update(double dt);
    void draw();

    const PrototypeID getPrototypeId() const { return prototypeId; }
    const Vec2& getPosition() const { return position; }

    void setId(EntityID id);
private:
    EntityID id;
    PrototypeID prototypeId;
    Vec2 position;
};
