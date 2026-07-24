#include <cstdint>

class Inventory {
public:
    Inventory(uint16_t slot_count);
    Inventory(uint16_t slot_count, std::vector<Item>);
};