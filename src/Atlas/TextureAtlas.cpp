#include <iostream>
#include <unordered_set>
#include <cstring>
#include <algorithm>
#include <set>
#include <fstream>

#include <Atlas/TextureAtlas.h>
#include <Lua/LuaHelper.h>
#include <Prototype/PrototypeRegister.h>
#include <EntitiesPrototypes/FurnacePrototype.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <bgfx/bimg/3rdparty/stb/stb_image.h>

void collectImagePathsFromField(const sol::object& field, std::set<std::string>& paths) {
    if (!field.valid())
        return;

    if (field.is<std::string>()) {
        paths.insert(resolveLuaPath(field.as<std::string>()));
    } else if (field.is<sol::table>()) {
        sol::table tbl = field;

        for (const char* key : { "filename", "spritesheet" }) {
            sol::object obj = tbl[key];
            if (obj.valid() && obj.is<std::string>()) {
                paths.insert(resolveLuaPath(obj.as<std::string>()));
            }
        }

        for (const char* key : { "layers", "main" }) {
            sol::object arrayObj = tbl[key];
            if (arrayObj.valid() && arrayObj.is<sol::table>()) {
                sol::table array = arrayObj;
                for (auto& kv : array) {
                    collectImagePathsFromField(kv.second, paths);
                }
            }
        }

        for (auto& kv : tbl) {
            collectImagePathsFromField(kv.second, paths);
        }
    }
}

bool TextureAtlas::buildFromLua(sol::state& lua, const std::string& entityType) {
    sol::table raw = lua["data"]["raw"];
    sol::table entities = raw[entityType];

    if (!entities.valid()) {
        std::cerr << "[TextureAtlas] Entity type '" << entityType << "' not found in Lua data.\n";
        return false;
    }

    std::set<std::string> sortedPaths;
PrototypeID pi;
// TBD: Temporary
for (auto& kv : entities) {
    sol::object& value = kv.second;
    if (!value.is<sol::table>()) continue;
    sol::table def = value.as<sol::table>();

    if(entityType == "furnace") {
    auto proto = std::make_unique<FurnacePrototype>(def);
    pi = PrototypeRegister::getInstance().AddPrototype(std::move(proto));

    }
    sol::object picture  = def["picture"];
    collectImagePathsFromField(picture, sortedPaths);

    sol::object variants = def["variants"];
    collectImagePathsFromField(variants, sortedPaths);
}

for (const auto& path : sortedPaths) {
    TextureAtlasSystem::getInstance().requestTexture(path);
}


    return true;
}


int TextureAtlas::getAtlasHeight() const {
    return atlasHeight;
}

int TextureAtlas::getAtlasWidth() const {
    return atlasWidth;
}

const TextureAtlas::UVRect& TextureAtlas::getUV(int id) const {
    static UVRect dummy{0, 0, 1, 1};
    auto it = idToUV.find(id);
    return it != idToUV.end() ? it->second : dummy;
}

int TextureAtlas::getTextureId(const std::string& name) const {
    auto it = nameToId.find(name);
    return it != nameToId.end() ? it->second : -1;
}

const std::string& TextureAtlas::getTextureName(int id) const {
    return id >= 0 && id < (int)idToName.size() ? idToName[id] : idToName[0];
}

bgfx::TextureHandle TextureAtlas::getAtlasTexture() const {
    return atlasHandle;
}
