#pragma once
#include <Camera/Camera.h>
#include <Renderer/RenderQueue.h>
#include <Tile/TileTextureManager.h>

class Surface;

void statate_to_render(Camera *camera, TileTextureManager *ttm, RenderQueue& renderQueue, Surface *surface);