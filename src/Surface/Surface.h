#pragma once

#include <Camera/Camera.h>
#include <Atlas/TextureAtlasSystem.h>
#include <Entity/EntityManager.h>
#include <Chunk.h>
#include <Tile/TileTextureManager.h>
#include <Renderer/RenderQueue.h>

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
    const std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash>& getChunks() const {
        return chunks;
    }

private:
    Camera *camera;
    TileTextureManager *ttm;
    RenderQueue renderQueue;
    std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash> chunks;

    bgfx::UniformHandle s_texColorUniform;
    bgfx::UniformHandle u_uvRectHandle;
    bgfx::TextureHandle texture;
};
