#pragma once
#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include <Prototype/PrototypeBase.h>

typedef uint32_t PrototypeID;

class PrototypeRegister {
public:
    static PrototypeRegister& getInstance() {
        static PrototypeRegister instance;
        return instance;
    }

    PrototypeID AddPrototype(std::unique_ptr<PrototypeBase> proto);
    const std::string& GetNameByID(PrototypeID id) const;
    const PrototypeID GetIdByName(const std::string& name) const;
    const PrototypeBase *GetPrototypeByName(const std::string& name) const;
    const PrototypeBase *GetPrototypeByID(PrototypeID id) const;
private:
    PrototypeRegister() = default;
    PrototypeRegister(const PrototypeRegister&) = delete;
    PrototypeRegister& operator=(const PrototypeRegister&) = delete;
    PrototypeRegister(PrototypeRegister&&) = delete;
    PrototypeRegister& operator=(PrototypeRegister&&) = delete;
    std::map<std::string, PrototypeID> name_to_id;
    std::map<PrototypeID, std::string> id_to_name;
    std::map<PrototypeID, std::unique_ptr<PrototypeBase>> id_to_prototype;
    PrototypeID next_id_ = 0;
};