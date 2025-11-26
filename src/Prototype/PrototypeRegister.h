#pragma once
#include <cstdint>
#include <map>
#include <memory>
#include <string>

class PrototypeBase;

typedef uint32_t PrototypeID;

class PrototypeRegister {
public:
    PrototypeRegister() = default;
    PrototypeID AddPrototype(std::unique_ptr<PrototypeBase> proto);
    const PrototypeID GetIdByName(const std::string& name) const;
    const PrototypeBase *GetPrototypeByName(const std::string& name) const;
    const PrototypeBase *GetPrototypeByID(PrototypeID id) const;
private:
     std::map<std::string, PrototypeID> name_to_id;
     std::map<PrototypeID, std::unique_ptr<PrototypeBase>> id_to_prototype;
     PrototypeID next_id_ = 0;
};