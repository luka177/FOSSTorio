#include <Visualisations/TransportBeltAnimationSet.h>

TransportBeltAnimationSet::TransportBeltAnimationSet(sol::table transportbeltanimationset) {
    if(transportbeltanimationset["animation_set"].valid()) {
        std::cout << "[TransportBeltAnimationSet] Valid TransportBeltAnimationSet\n";
        animation_set = RotatedAnimation(transportbeltanimationset["animation_set"].get<sol::table>());
    }
};