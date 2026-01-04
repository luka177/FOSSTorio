#pragma once

#include <Camera/Camera.h>
#include <Atlas/TextureAtlasSystem.h>
#include <Entity/EntityManager.h>
#include <Chunk.h>
#include <Tile/TileTextureManager.h>
#include <Renderer/RenderQueue.h>
#include <misc.h>

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
    std::optional<Entity> FindEntityByCoords(int64_t x, int64_t y);
    template<class T>
    std::optional<Entity> FindEntityByCoords(const Vec2T<T>& global_coord) {
     const int64_t x = static_cast<int64_t>(std::floor(global_coord.x));
     const int64_t y = static_cast<int64_t>(std::floor(global_coord.y));

        return FindEntityByCoords(x, y);
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
