#include <iostream>

#include <Prototype/PrototypeRegister.h>
#include <Prototype/PrototypeBase.h>

PrototypeID PrototypeRegister::AddPrototype(std::unique_ptr<PrototypeBase> proto) {
    PrototypeID id = next_id_++;
    const std::string& name = proto->getName();
    name_to_id[name] = id;
    proto->setId(id);
    id_to_prototype[id] = std::move(proto);
    std::cout << "[PrototypeRegister] Added prototype " << name
              << " id: " << id << "\n";
    return id;
}

const PrototypeID PrototypeRegister::GetIdByName(const std::string& name) const {
    auto it = name_to_id.find(name);
    if (it == name_to_id.end()) {
        throw std::out_of_range("[PrototypeRegister] No prototype with name: " + name);
    }
    return it->second;
}

const PrototypeBase* PrototypeRegister::GetPrototypeByName(const std::string& name) const {
    auto it = name_to_id.find(name);
    if (it == name_to_id.end()) return nullptr;

    auto it_proto = id_to_prototype.find(it->second);
    if (it_proto == id_to_prototype.end()) return nullptr;

    return it_proto->second.get();
}

const PrototypeBase* PrototypeRegister::GetPrototypeByID(PrototypeID id) const {
    auto it = id_to_prototype.find(id);
    if (it == id_to_prototype.end()) return nullptr;
    return it->second.get();
}