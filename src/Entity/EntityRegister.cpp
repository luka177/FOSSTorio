#include <iostream>

#include <Entity/EntityRegister.h>
#include <Entity/Entity.h>

EntityID EntityRegister::AddEntity(std::unique_ptr<Entity> entity) {
    EntityID id;
    if (!free_ids_.empty()) {
        id = free_ids_.front();
        free_ids_.pop();
    } else {
        id = next_id_++;
    }
    entity->setId(id);
    id_to_entity[id] = std::move(entity);
    return id;
}

const Entity* EntityRegister::GetEntityByID(EntityID id) const {
    auto it = id_to_entity.find(id);
    if (it == id_to_entity.end()) return nullptr;
    return it->second.get();
}

bool EntityRegister::RemoveEntity(EntityID id) {
    size_t erased_count = id_to_entity.erase(id);
    if (erased_count > 0) {
        free_ids_.push(id);
        return true;
    }
    return false;
}