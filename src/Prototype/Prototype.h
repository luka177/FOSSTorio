#pragma once
#include <Prototype/PrototypeBase.h>

class Prototype: public PrototypeBase {
public:
    Prototype(sol::table entities);
    virtual ~Prototype() = default;
protected:
    //string factoriopedia_alternative;
    //CustomTooltipField[] custom_tooltip_fields;
};