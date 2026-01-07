#pragma once

#include <vector>

#include <Prototype/PrototypeRegister.h>

using ItemId = uint32_t;

struct Item {
    std::optional<PrototypeID> prototype;
};


class ItemRegister {
public:
    static ItemRegister& getInstance() {
        static ItemRegister instance;
        return instance;
    }

    Item& Get(ItemId id) { return items[id]; }
    ItemId Add(const Item& item)
    {
        items.push_back(item);
        return items.size() - 1;
    }


private:
    ItemRegister() = default;
    ItemRegister(const ItemRegister&) = delete;
    ItemRegister& operator=(const ItemRegister&) = delete;
    ItemRegister(ItemRegister&&) = delete;
    ItemRegister& operator=(ItemRegister&&) = delete;

    std::vector<Item> items;
};