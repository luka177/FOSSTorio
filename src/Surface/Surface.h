#pragma once

#include <Camera/Camera.h>
#include <Atlas/TextureAtlasSystem.h>
#include <Entity/Entity.h>
#include <Chunk.h>
#include <Tile/TileTextureManager.h>

#include <sol/sol.hpp>
#include <bgfx/bgfx.h>

struct TileChunkCoord {
    struct TileCoord tile;
    struct ChunkCoord chunk;
};

struct GlobalCoord {
    int x;
    int y;
};

class Surface {
public:
    Surface(Camera *camera, TileTextureManager *ttm);
    void create_entity(sol::table args);

    void update(double dt);
    void draw(bgfx::VertexBufferHandle vbo, bgfx::IndexBufferHandle ibo, bgfx::ProgramHandle program);
    struct TileChunkCoord getTileChunkCoord(int globalX, int globalY);
    struct TileChunkCoord getTileChunkCoord(struct GlobalCoord globalCoord);

private:
    Camera *camera;
    TileTextureManager *ttm;
    std::vector<std::unique_ptr<Entity>> entities;
    std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash> chunks;

    bgfx::UniformHandle s_texColorUniform;
    bgfx::UniformHandle u_uvRectHandle;
    bgfx::TextureHandle texture;
};
