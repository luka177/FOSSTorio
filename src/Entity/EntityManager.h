#pragma once
#include <bitset>
#include <cassert>
#include <cstdint>
#include <vector>

using Signature = std::bitset<128>;
typedef uint64_t Entity;

class EntityManager
{
public:
    Entity CreateEntity()
    {
        Entity id;

        if (!mFreeEntities.empty())
        {
            id = mFreeEntities.back();
            mFreeEntities.pop_back();
        }
        else
        {
            id = static_cast<Entity>(mSignatures.size());
            mSignatures.emplace_back();
        }

        return id;
    }

    void DestroyEntity(Entity entity)
    {
        assert(entity < mSignatures.size() && "Entity out of range.");

        mSignatures[entity].reset();
        mFreeEntities.push_back(entity);
    }

    void SetSignature(Entity entity, Signature signature)
    {
        assert(entity < mSignatures.size() && "Entity out of range.");
        mSignatures[entity] = signature;
    }

    Signature GetSignature(Entity entity) const
    {
        assert(entity < mSignatures.size() && "Entity out of range.");
        return mSignatures[entity];
    }

private:
    std::vector<Signature> mSignatures{};
    std::vector<Entity> mFreeEntities{};
};