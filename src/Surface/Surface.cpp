#include <iostream>

#include <Surface/Surface.h>
#include <Renderer/Renderer.h>
#include <Atlas/TextureAtlasSystem.h>

#include <bgfx/bgfx.h>
#include <bx/math.h>

Surface::Surface(Camera *camera, TileTextureManager *ttm) : camera(camera), ttm(ttm) {
    // TBD properly generate surface, for now just create chunks manually
    const int halfSize = 5;
    for (int y = -halfSize; y < halfSize; ++y) {
        for (int x = -halfSize; x < halfSize; ++x) {
            ChunkCoord coord = {x, y};
            chunks[coord] = std::make_unique<Chunk>();
        }
    }
    u_uvRectHandle = bgfx::createUniform("u_uvRect", bgfx::UniformType::Vec4);
    s_texColorUniform = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
    texture = Renderer::loadTexture("tile.png"); // Your texture loader

    if (!bgfx::isValid(texture)) {
      std::cerr << "Texture load failed!" << std::endl;
    } else {
      std::cerr << "Texture load ok!" << std::endl;
    }
}

void Surface::create_entity(sol::table args) {
    std::string name = args["name"];
    sol::table pos = args["position"];
    double x = pos["x"];
    double y = pos["y"];

    Vec2 position{x, y};

    std::cout << "[Surface] Creating entity: " << name << " at (" << x << ", " << y << ")" << std::endl;

    entities.push_back(std::make_unique<Entity>(name, position));
}

  struct NormalColorVertex {
    float x, y;
    float u, v;
};

void Surface::update(double dt) {
    for (auto& entity : entities) {
        entity->update(dt);
    }
}

void Surface::draw(bgfx::VertexBufferHandle vbo, bgfx::IndexBufferHandle ibo, bgfx::ProgramHandle program) {
    const float tileSize = 32.0f;

    float camX = camera->getX();
    float camY = camera->getY();
    float zoom = camera->getZoom();

//    bgfx::dbgTextPrintf(0, 0, 0x0f, "Camera X: %d, Y: %d", camX, camY);
  //  std::cout << "Camera X:" << camX << " Y: " <<camY <<  std::endl;
    // Get actual window (backbuffer) size
    const bgfx::Stats* stats = bgfx::getStats();
    float windowWidth = static_cast<float>(stats->width);
    float windowHeight = static_cast<float>(stats->height);

    // Compute visible world area
    float halfViewW = (windowWidth * 0.5f) / zoom;
    float halfViewH = (windowHeight * 0.5f) / zoom;

    int minTileX = static_cast<int>(std::floor((camX - halfViewW) / tileSize));
    int maxTileX = static_cast<int>(std::ceil((camX + halfViewW) / tileSize));
    int minTileY = static_cast<int>(std::floor((camY - halfViewH) / tileSize));
    int maxTileY = static_cast<int>(std::ceil((camY + halfViewH) / tileSize));

    float atlasCols = 64.0f;
    float atlasRows = 9.0f;

    int tileType, tx, ty;

    // Add padding if needed (e.g., half-texel inset)
    float padding = 0.0f;
    float tileW = 1.0f / atlasCols;
    float tileH = 1.0f / atlasRows;

    float u0, u1, v0, v1;

    std::array<float, 4> uvRect;

    for (int y = minTileY; y <= maxTileY; ++y) {
        for (int x = minTileX; x <= maxTileX; ++x) {
            // Convert global tile coord to chunk/tile-in-chunk
            TileChunkCoord coord = getTileChunkCoord(x, y);
            ChunkCoord chunkCoord = coord.chunk;

            auto it = chunks.find(chunkCoord);
            if (it == chunks.end()) {
                continue; // Chunk doesn't exist, skip
            }

            Chunk* chunk = it->second.get();
            Tile& tile = chunk->getTile(coord.tile.tileX, coord.tile.tileY);

            auto info = ttm->getTileUVInfo("grass-1", 1, tile.getTextureId());

            float uvRect[4] = { info.uv.u0, info.uv.v0, info.uv.u1, info.uv.v1 };
            bgfx::setUniform(u_uvRectHandle, uvRect);

            float model[16];
            bx::mtxSRT(model,
                tileSize, tileSize, 1.0f,
                0.0f, 0.0f, 0.0f,
                x * tileSize, y * tileSize, 0.0f);

            // TODO: set correct texture for tile.get_type()
            bgfx::setTexture(0, s_texColorUniform,
                 TextureAtlasSystem::getInstance().getAtlasTexture(info.atlasId));
            bgfx::setTransform(model);
            bgfx::setVertexBuffer(0, vbo);
            bgfx::setIndexBuffer(ibo);
            bgfx::submit(1, program);
        }
    }

    for (const auto& entity : entities) {
        const Vec2 pos = entity->getPosition();
        const struct AtlasUV& uv = TextureAtlasSystem::getInstance().getUV(27);
        float uvRect[4] = { uv.u0, uv.v0, uv.u1, uv.v1 };
        bgfx::setUniform(u_uvRectHandle, uvRect);
        float model[16];
        bx::mtxSRT(model,
            tileSize, tileSize, 1.0f,
            0.0f, 0.0f, 0.0f,
            pos.x * tileSize, pos.y * tileSize, 0.0f);
        bgfx::setTexture(0, s_texColorUniform,  TextureAtlasSystem::getInstance().getAtlasTexture(uv.atlasId));
        bgfx::setTransform(model);
        bgfx::setVertexBuffer(0, vbo);
        bgfx::setIndexBuffer(ibo);
        bgfx::submit(0, program);

    }
}

struct TileChunkCoord Surface::getTileChunkCoord(int globalX, int globalY) {
    TileChunkCoord result;

    result.chunk.chunkX = (globalX >= 0) ? (globalX / CHUNK_SIZE) : ((globalX + 1) / CHUNK_SIZE - 1);
    result.chunk.chunkY = (globalY >= 0) ? (globalY / CHUNK_SIZE) : ((globalY + 1) / CHUNK_SIZE - 1);

    result.tile.tileX = globalX - result.chunk.chunkX * CHUNK_SIZE;
    result.tile.tileY = globalY - result.chunk.chunkY * CHUNK_SIZE;

    return result;
}

struct TileChunkCoord Surface::getTileChunkCoord(struct GlobalCoord globalCoord) {
    return getTileChunkCoord(globalCoord.x, globalCoord.y);
}
