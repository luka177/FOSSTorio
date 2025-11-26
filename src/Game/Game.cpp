#include <Game/Game.h>

Game::Game(Camera *camera, TileTextureManager *ttm)
  : camera(camera), ttm(ttm){}

void Game::bind(sol::state& lua) {
    lua.new_usertype<Surface>("Surface",
        "create_entity", &Surface::create_entity
    );
    lua.new_usertype<Game>("Game",
        "surfaces", &Game::surfaces
    );
    surfaces = lua.create_table();
    nauvis = new Surface(camera, ttm);
    surfaces["nauvis"] = nauvis;

    lua["game"] = this;
    lua["game"]["surfaces"] = surfaces;
}
