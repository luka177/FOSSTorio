#include <iostream>

#include <Lua/LuaEngine.h>

LuaEngine::LuaEngine(Game *game)
    : scriptLoaded(false), game(game)
{
}

LuaEngine::~LuaEngine() {
    shutdown();
}

bool LuaEngine::initialize() {
    lua.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::math,
        sol::lib::table,
        sol::lib::string
    );

    sol::state_view luaState(lua);

    sol::table data = lua.create_named_table("data");
    sol::table raw = luaState.create_table();
    data["raw"] = raw;

    sol::table utility_sounds = luaState.create_table();
    utility_sounds["default"] = luaState.create_table();
    raw["utility-sounds"] = utility_sounds;

    data.set_function("extend", [](sol::this_state ts, sol::object, sol::table prototypeList) {
    sol::state_view lua(ts);
    sol::table data = lua["data"];
    sol::table raw = data["raw"];

    int count = 0;
    while (prototypeList[count + 1].valid()) ++count;
    std::cout << "[data:extend] Received " << count << " prototype(s):\n";

    for (int i = 1; i <= count; ++i) {
        sol::object entry = prototypeList[i];
        if (!entry.is<sol::table>()) continue;

        sol::table proto = entry.as<sol::table>();
        if (!proto["type"].valid() || !proto["name"].valid()) continue;

        std::string type = proto.get<std::string>("type");
        std::string name = proto.get<std::string>("name");

        std::cout << "  -> [" << type << "] " << name << "\n";

        if (!raw[type].valid()) {
            raw[type] = lua.create_table();
        }

        raw[type][name] = proto;
    }
});

    // patch `require()` to support __base__, __core__ ===
    lua.script(R"(
        local original_require = require

        function require(modpath)
            local rewritten = modpath

            local function normalize(path)
                return path:gsub("[/\\]", ".")
            end

            local core_match = string.match(modpath, "^__core__[./](.+)$")
            local base_match = string.match(modpath, "^__base__[./](.+)$")

            if core_match then
                rewritten = "data.core." .. normalize(core_match)
            elseif base_match then
                rewritten = "data.base." .. normalize(base_match)
            end

            return original_require(rewritten)
        end
    )");

    //setup module search paths ===
    lua.script(R"(
        package.path = table.concat({
            "data/?.lua",
            "data/?/init.lua",
            "data/base/?.lua",
            "data/base/?/init.lua",
            "data/core/?.lua",
            "data/core/lualib/?.lua",
            "data/core/lualib/?/init.lua",
            package.path
        }, ";")
    )");

    // HACK?
    lua.script(R"(
        defines = {
            direction = {
                north = 0, northnortheast = 1, northeast = 2, eastnortheast = 3,
                east = 4, eastsoutheast = 5, southeast = 6, southsoutheast = 7,
                south = 8, southsouthwest = 9, southwest = 10, westsouthwest = 11,
                west = 12, westnorthwest = 13, northwest = 14, northnorthwest = 15
            },
            inventory = {
                lab_modules = 1
            }
        }
    )");

    std::cout << "[LuaEngine] Initialized successfully.\n";
    return true;
}

void LuaEngine::shutdown() {
    scriptLoaded = false;
}

bool LuaEngine::loadScript(const std::string& filepath) {
    sol::load_result script = lua.load_file(filepath);

    if (!script.valid()) {
        sol::error err = script;
        std::cerr << "[LuaEngine] Failed to load Lua script: " << err.what() << std::endl;
        return false;
    }

    sol::protected_function_result result = script();
    if (!result.valid()) {
        sol::error err = result;
        std::cerr << "[LuaEngine] Failed to run Lua script: " << err.what() << std::endl;
        return false;
    }

    scriptLoaded = true;
    std::cout << "[LuaEngine] Lua script loaded: " << filepath << std::endl;
    return true;
}

void LuaEngine::callOnInit() {
    game->bind(lua);
    if (!scriptLoaded) return;
    sol::protected_function on_init = lua["on_init"];
    if (on_init.valid()) {
        on_init();
    }
}

void LuaEngine::callOnTick(double deltaTime) {
    if (!scriptLoaded) return;
    sol::protected_function on_tick = lua["on_tick"];
    if (on_tick.valid()) {
        on_tick(deltaTime);
    }
}

Game* LuaEngine::getGame() {
    return game.get();
}

sol::state& LuaEngine::getState() {
    return lua;
}
