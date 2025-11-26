#include <Tile/TileTexture.h>
#include <Lua/LuaHelper.h>

bool TileTexture::loadFromLua(sol::table tileDef) {
    name = tileDef["name"].get_or(std::string{});
    subgroup = tileDef["subgroup"].get_or(std::string{});
    type = tileDef["type"].get_or(std::string{});
    order = tileDef["order"].get_or(std::string{});
    layer = tileDef["layer"].get_or(0);

    if (name.empty()) {
        std::cerr << "[TileTexture] Skipping unnamed tile definition.\n";
        return false;
    }

    sol::object variantsObj = tileDef["variants"];
    if (!variantsObj.valid() || !variantsObj.is<sol::table>()) {
        std::cerr << "[TileTexture] Tile '" << name << "' has no valid 'variants' table.\n";
        return false;
    }

    sol::table variants = variantsObj;
    sol::object mainObj = variants["main"];
    if (!mainObj.valid() || !mainObj.is<sol::table>()) {
        std::cerr << "[TileTexture] Tile '" << name << "' has no valid 'main' table in variants.\n";
        return false;
    }

    sol::table main = mainObj;
    for (std::size_t i = 1; i <= main.size(); ++i) {
        sol::object varObj = main[i];
        if (!varObj.valid() || !varObj.is<sol::table>()) continue;

        sol::table var = varObj;
        int size = var["size"].get_or(1);
        TileVariation variation;
        if (variation.loadFromLua(var)) {
            variations[size] = variation;
        } else {
            std::cerr << "[TileTexture] Failed to load variation size " << size << " for tile '" << name << "'.\n";
        }
    }

    return !variations.empty();
}


const std::string& TileTexture::getName() const {
    return name;
}

const TileVariation* TileTexture::getVariation(int size) const {
    auto it = variations.find(size);
    if (it != variations.end()) return &it->second;
    return nullptr;
}
