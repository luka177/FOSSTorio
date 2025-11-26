#include <Prototype/PrototypeBase.h>

PrototypeBase::PrototypeBase(sol::table entity)
    : name(entity["name"].get<std::string>()){
    sol::object obj;
    obj = entity["name"];
    if (!obj.valid()) {
        throw std::runtime_error("[PrototypeBase] name isnt optional, but is missing!\n");
    }
    order = entity["order"].get_or(std::string{});
    hidden = entity["hidden"].get_or(false);
    hidden_in_factoriopedia = entity["hidden_in_factoriopedia"].get_or(false);
    parameter = entity["parameter"].get_or(false);
}

PrototypeID PrototypeBase::getId() const {
    return id;
}

void PrototypeBase::setId(PrototypeID id) {
    this->id = id;
}

const std::string& PrototypeBase::getName() const {
    return name;
}