#include <EntitiesPrototypes/TransportBeltPrototype.h>

TransportBeltPrototype::TransportBeltPrototype(sol::table entity)
    : TransportBeltConnectablePrototype(entity) {
    circuit_wire_max_distance = entity["circuit_wire_max_distance"].get_or(0);
    draw_copper_wires = entity["draw_copper_wires"].get_or(true);
    draw_circuit_wires = entity["draw_circuit_wires"].get_or(true);
    related_underground_belt = entity["related_underground_belt"].get_or(std::string{});

    std::cout << "[TransportBeltPrototype] Created entity prototype " << name << "\n";
}
