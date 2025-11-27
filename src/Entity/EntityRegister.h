#pragma once
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <queue>

#include <Entity/Entity.h>

typedef uint32_t EntityID;

class EntityRegister {
public:
    static EntityRegister& getInstance() {
        static EntityRegister instance;
        return instance;
    }

    EntityID AddEntity(std::unique_ptr<Entity> entity);
    const Entity *GetEntityByID(EntityID id) const;
    bool RemoveEntity(EntityID id);

private:
    EntityRegister() = default;
    EntityRegister(const EntityRegister&) = delete;
    EntityRegister& operator=(const EntityRegister&) = delete;
    EntityRegister(EntityRegister&&) = delete;
    EntityRegister& operator=(EntityRegister&&) = delete;
    std::map<EntityID, std::unique_ptr<Entity>> id_to_entity;
    EntityID next_id_ = 0;
    std::queue<EntityID> free_ids_;
};