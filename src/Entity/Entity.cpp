#include <iostream>

#include <Entity/Entity.h>

Entity::Entity(const std::string& name, const Vec2& position)
    : name(name), position(position)
{
    std::cout << "[Entity] Created " << name << " at (" << position.x << ", " << position.y << ")\n";
}

void Entity::update(double dt) {

}

void Entity::draw() {
    // TBD: Figure how we do it
}
