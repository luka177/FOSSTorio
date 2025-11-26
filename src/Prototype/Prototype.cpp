#include <Prototype/Prototype.h>

Prototype::Prototype(sol::table entity)
    :PrototypeBase(entity) {
    std::cout << "[Prototype] Created entity prototype " << name << "\n";
}