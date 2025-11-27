#include <iostream>

#include <Entity/Entity.h>
#include <Entity/EntityRegister.h>

Entity::Entity(const PrototypeID prototypeId, const Vec2& position)
    : prototypeId(prototypeId), position(position)
{
    std::cout << "[Entity] Created " << PrototypeRegister::getInstance().GetNameByID(prototypeId) << " at (" << position.x << ", " << position.y << ")\n";
}

void Entity::update(double dt) {

}

void Entity::draw() {
    // TBD: Figure how we do it
}

void Entity::setId(EntityID id) {
    this->id = id;
}