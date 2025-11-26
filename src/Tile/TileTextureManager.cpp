#include <algorithm>

#include <Tile/TileTextureManager.h>

void TileTextureManager::loadFromLua(sol::state& lua) {
    sol::object dataObj = lua["data"];
    if (!dataObj.valid() || !dataObj.is<sol::table>()) {
        std::cerr << "[TileTextureManager] 'data' table is missing or invalid\n";
        return;
    }

    sol::table data = dataObj;
    sol::object rawObj = data["raw"];
    if (!rawObj.valid() || !rawObj.is<sol::table>()) {
        std::cerr << "[TileTextureManager] 'data.raw' is missing or invalid\n";
        return;
    }

    sol::table raw = rawObj;
    sol::object tilesObj = raw["tile"];
    if (!tilesObj.valid() || !tilesObj.is<sol::table>()) {
        std::cerr << "[TileTextureManager] 'data.raw.tile' is missing or invalid\n";
        return;
    }

    sol::table tiles = tilesObj;

    // Gather and sort tile names for deterministic order
    std::vector<std::string> sortedNames;
    for (auto& kv : tiles) {
        if (kv.first.is<std::string>()) {
            sortedNames.push_back(kv.first.as<std::string>());
        }
    }
    std::sort(sortedNames.begin(), sortedNames.end());

    int fixedId = 0;
    for (const std::string& name : sortedNames) {
        sol::table tileDef = tiles[name];
        if (!tileDef.valid()) continue;

        TileTexture tex;
        tex.loadFromLua(tileDef);

        nameToIndex[tex.getName()] = static_cast<int>(tileTextures.size());
        tileNameToFixedId[tex.getName()] = fixedId++;
        fixedTileNames.push_back(tex.getName());
        tileTextures.push_back(tex);
    }

    std::cout << "[TileTextureManager] Loaded " << tileTextures.size() << " tile(s)\n";
}

const UVRect TileTextureManager::getUV(const std::string& name, int size, int index) const {
    auto it = nameToIndex.find(name);
    if (it == nameToIndex.end()) return UVRect{0, 0, 0, 0};
    return getUV(it->second, size, index);
}

const UVRect TileTextureManager::getUV(int tileId, int size, int index) const {
    if (tileId < 0 || tileId >= (int)tileTextures.size()) return UVRect{0, 0, 0, 0};

    const TileTexture& tex = tileTextures[tileId];
    const TileVariation* variation = tex.getVariation(size);
    if (!variation || index < 0 || index >= variation->getCount()) return UVRect{0, 0, 0, 0};

    return variation->getUV(index);
}

TileUVInfo TileTextureManager::getTileUVInfo(const std::string& name, int size, int index) const {
    auto it = nameToIndex.find(name);
    if (it == nameToIndex.end()) return {0, {0,0,0,0}};

    const TileTexture& tex = tileTextures[it->second];
    const TileVariation* var = tex.getVariation(size);

    if (!var) return {0, {0,0,0,0}};
    TextureId tid = var->getTextureId();

    const AtlasUV& auv = TextureAtlasSystem::getInstance().getUV(tid);
    UVRect rect = var->getUV(index);

    return { auv.atlasId, rect };
}
