#pragma once
#include <string>

#include <EntitiesPrototypes/TransportBeltConnectablePrototype.h>

class TransportBeltPrototype: public TransportBeltConnectablePrototype {
public:
    TransportBeltPrototype(sol::table entities);
    virtual ~TransportBeltPrototype() = default;
protected:
    //TransportBeltConnectorFrame connector_frame_sprites;
    double circuit_wire_max_distance = 0;
    bool draw_copper_wires = true;
    bool draw_circuit_wires = true;
    //circuit_connector TBD
    std::string related_underground_belt;
};
