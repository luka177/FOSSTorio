#pragma once

#include <string>

#include <Game/Game.h>

#include <sol/sol.hpp>

class LuaEngine {
public:
    LuaEngine(Game *game);
    ~LuaEngine();

    bool initialize();
    void shutdown();

    bool loadScript(const std::string& filepath);

    void callOnInit();
    void callOnTick(double deltaTime);
    Game* getGame();
    sol::state& getState();

private:
    sol::state lua;
    bool scriptLoaded;
    std::unique_ptr<Game> game;
};
