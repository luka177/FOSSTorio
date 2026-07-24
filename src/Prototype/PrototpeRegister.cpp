#include <iostream>

#include <Prototype/PrototypeRegister.h>
#include <Prototype/PrototypeBase.h>
#include <Item/ItemPrototype.h>

PrototypeID PrototypeRegister::AddPrototype(std::unique_ptr<PrototypeBase> proto) {
    PrototypeID id = next_id_++;
    const std::string& name = proto->getName();

    std::map<std::string, PrototypeID>& targetMap =
        dynamic_cast<ItemPrototype*>(proto.get()) ? item_name_to_id : name_to_id;

    if (targetMap.find(name) == targetMap.end()) {
        targetMap[name] = id;
    } else {
        std::cerr << "[PrototypeRegister] Duplicate prototype name \"" << name
                  << "\", GetIdByName/GetItemIdByName will keep resolving to id " << targetMap[name]
                  << " (this new id " << id << " is still reachable by id)\n";
    }

    id_to_name[id] = name;
    proto->setId(id);
    id_to_prototype[id] = std::move(proto);
    std::cout << "[PrototypeRegister] Added prototype " << name
              << " id: " << id << "\n";
    return id;
}

const std::string& PrototypeRegister::GetNameByID(PrototypeID id) const {
    auto it = id_to_name.find(id);

    if (it == id_to_name.end()) {
        throw std::out_of_range("[PrototypeRegister] No name registered for ID: " + std::to_string(id));
    }

    return it->second;
}

const PrototypeID PrototypeRegister::GetIdByName(const std::string& name) const {
    auto it = name_to_id.find(name);
    if (it == name_to_id.end()) {
        throw std::out_of_range("[PrototypeRegister] No prototype with name: " + name);
    }
    return it->second;
}

const PrototypeID PrototypeRegister::GetItemIdByName(const std::string& name) const {
    auto it = item_name_to_id.find(name);
    if (it == item_name_to_id.end()) {
        throw std::out_of_range("[PrototypeRegister] No item prototype with name: " + name);
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