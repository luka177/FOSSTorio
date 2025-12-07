#pragma once
#include <cassert>
#include <cstdint>
#include <limits>
#include <memory>
#include <vector>

#include <Entity/ComponentArray.h>

using ComponentType = std::uint16_t;
inline constexpr ComponentType INVALID_COMPONENT = std::numeric_limits<ComponentType>::max();


class ComponentManager
{
public:
    template<typename T>
    ComponentType RegisterComponent()
    {
        assert(GetComponentTypeInternal<T>() == INVALID_COMPONENT &&
               "registering component type more than once");

        ComponentType id = mNextComponentType++;
        SetComponentType<T>(id);

        if (mComponentArrays.size() <= id)
            mComponentArrays.resize(id + 1);

        assert(!mComponentArrays[id] && "component array slot already used");
        mComponentArrays[id] = std::make_shared<ComponentArray<T>>();

        return id;
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        ComponentType id = GetComponentTypeInternal<T>();
        assert(id != INVALID_COMPONENT && "Component not registered before use.");
        return id;
    }

    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        GetComponentArray<T>()->InsertData(entity, component);
    }

    template<typename T>
    void RemoveComponent(Entity entity)
    {
        GetComponentArray<T>()->RemoveData(entity);
    }

    template<typename T>
    T& GetComponent(Entity entity)
    {
        return GetComponentArray<T>()->GetData(entity);
    }

    void EntityDestroyed(Entity entity)
    {
        for (auto& arr : mComponentArrays)
        {
            if (arr)
                arr->EntityDestroyed(entity);
        }
    }

    template<typename T>
    ComponentArray<T>& GetComponentArrayRef()
    {
        return *GetComponentArray<T>();
    }

    template<typename T>
    bool HasComponent(Entity entity)
    {
        return GetComponentArray<T>()->Has(entity);
    }

private:

    template<typename T>
    static ComponentType& GetComponentTypeInternal()
    {
        static ComponentType type = INVALID_COMPONENT;
        return type;
    }

    template<typename T>
    static void SetComponentType(ComponentType id)
    {
        GetComponentTypeInternal<T>() = id;
    }

    std::vector<std::shared_ptr<IComponentArray>> mComponentArrays{};
    ComponentType mNextComponentType{};

    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray()
    {
        ComponentType id = GetComponentType<T>();
        assert(id >= 0 && id < static_cast<ComponentType>(mComponentArrays.size()));
        assert(mComponentArrays[id] && "Component not registered before use.");

        return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[id]);
    }
};
