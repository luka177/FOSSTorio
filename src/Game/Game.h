#pragma once
#include <Surface/Surface.h>
#include <Camera/Camera.h>
#include <Tile/TileTextureManager.h>

#include <sol/sol.hpp>

class Game {
public:
    Game(Camera *camera, TileTextureManager *ttm);
    sol::table surfaces;

    void bind(sol::state& lua);
    Surface* nauvis;
private:
    Camera *camera;
    TileTextureManager *ttm;
};
