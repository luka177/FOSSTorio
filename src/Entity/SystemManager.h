#pragma once
#include <cassert>
#include <cstdint>
#include <limits>
#include <memory>
#include <vector>
#include <algorithm>

#include <Entity/EntityManager.h>
#include <Entity/System.h>

using SystemType = std::uint8_t;
inline constexpr SystemType INVALID_SYSTEM = std::numeric_limits<SystemType>::max();
inline constexpr std::size_t MAX_SYSTEMS = 32;

class SystemManager
{
public:
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        assert(GetSystemTypeInternal<T>() == INVALID_SYSTEM && "Registering system more than once.");

        assert(mNextSystemType < MAX_SYSTEMS && "too many systems :()");
        SystemType id = mNextSystemType++;
        SetSystemType<T>(id);

        if (mSystems.size() <= id)
        {
            mSystems.resize(id + 1);
            mSignatures.resize(id + 1);
        }

        auto system = std::make_shared<T>();
        mSystems[id] = system;
        mSignatures[id].reset();
        return system;
    }

    template<typename T>
    void SetSignature(Signature signature)
    {
        SystemType id = GetSystemType<T>();
        assert(id != INVALID_SYSTEM && "System used before registered.");
        assert(id < mSignatures.size());
        mSignatures[id] = signature;
    }

    void EntityDestroyed(Entity entity)
    {
        for (auto& sys : mSystems)
        {
            if (sys)
                sys->mEntities.erase(std::remove(sys->mEntities.begin(), sys->mEntities.end(), entity), sys->mEntities.end());
        }
    }

    void EntitySignatureChanged(Entity entity, Signature entitySignature)
    {
        for (SystemType id = 0; id < mSystems.size(); ++id)
        {
            auto& sys = mSystems[id];
            if (!sys) continue;

            const auto& systemSignature = mSignatures[id];

            if ((entitySignature & systemSignature) == systemSignature)
                sys->mEntities.push_back(entity);
            else
                sys->mEntities.erase(std::remove(sys->mEntities.begin(), sys->mEntities.end(), entity), sys->mEntities.end());
        }
    }

private:
    template<typename T>
    static SystemType& GetSystemTypeInternal()
    {
        static SystemType type = INVALID_SYSTEM;
        return type;
    }

    template<typename T>
    static void SetSystemType(SystemType id)
    {
        GetSystemTypeInternal<T>() = id;
    }

    template<typename T>
    SystemType GetSystemType() const
    {
        SystemType id = GetSystemTypeInternal<T>();
        assert(id != INVALID_SYSTEM);
        return id;
    }

private:
    std::vector<Signature> mSignatures{};
    std::vector<std::shared_ptr<System>> mSystems{};
    SystemType mNextSystemType{0};
};