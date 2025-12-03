#pragma once

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};


template<typename T>
class ComponentArray : public IComponentArray
{
public:
    void InsertData(Entity entity, T component)
    {
        assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");

        // Put new entry at end and update the maps
        size_t newIndex = mComponentArray.size();
        mEntityToIndexMap[entity] = newIndex;
        mComponentArray.push_back(component);
        mIndexToEntityMap.push_back(entity);
    }

    void RemoveData(Entity entity)
    {
        assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

        // Copy element at end into deleted element's place to maintain density
        size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
        size_t indexOfLastElement   = mComponentArray.size() - 1;

        if (indexOfRemovedEntity != indexOfLastElement)
        {
            mComponentArray[indexOfRemovedEntity] = std::move(mComponentArray[indexOfLastElement]);

            Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
            mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;
            mEntityToIndexMap[entityOfLastElement]  = indexOfRemovedEntity;
        }

        // Update map to point to moved spot
        mComponentArray.pop_back();
        mIndexToEntityMap.pop_back();

        mEntityToIndexMap.erase(entity);
    }

    T& GetData(Entity entity)
    {
        assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");

        // Return a reference to the entity's component
        return mComponentArray[mEntityToIndexMap[entity]];
    }

    void EntityDestroyed(Entity entity) override
    {
        auto it = mEntityToIndexMap.find(entity);
        if (it != mEntityToIndexMap.end())
        {
            RemoveData(entity);
        }
    }

private:
    std::vector<T> mComponentArray;
    std::unordered_map<Entity, size_t> mEntityToIndexMap;
    std::vector<Entity> mIndexToEntityMap;
};