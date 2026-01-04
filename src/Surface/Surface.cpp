#include <iostream>

#include <Animator/Animator.h>
#include <Surface/Surface.h>
#include <Renderer/Renderer.h>
#include <Atlas/TextureAtlasSystem.h>
#include <Prototype/PrototypeRegister.h>
#include <Entity/Coordinator.h>
#include <EntitiesPrototypes/CraftingMachinePrototype.h>
#include <EntitiesPrototypes/TransportBeltConnectablePrototype.h>
#include <Animation/AnimationParameters.h>
#include <Simulation/BeltMovementSystem.h>
#include <Simulation/InserterSystem.h>
#include <misc.h>

#include <bgfx/bgfx.h>
#include <bx/math.h>
bool done=false;
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
    int64_t x = pos["x"];
    int64_t y = pos["y"];
    Direction dir = args["direction"].get_or(Direction::North);
    Vec2 position{x, y};

    std::cout << "[Surface] Creating entity: " << name << " at (" << x << ", " << y << ") direction: " << dir << std::endl;
    Entity toadd = Coordinator::Instance().CreateEntity();
    AnimationFrameComponent frameid;
    frameid.frame = 0;
    Coordinator::Instance().AddComponent(toadd, position);
    Coordinator::Instance().AddComponent(toadd, frameid);
    Coordinator::Instance().AddComponent(toadd, dir);
    Coordinator::Instance().AddComponent(toadd, this);
    Coordinator::Instance().AddComponent(toadd, PrototypeRegister::getInstance().GetIdByName(name));
        if(name=="fast-transport-belt") {
            if (!done) {
                BeltComponent belt{};
                PrototypeID id = PrototypeRegister::getInstance().GetIdByName("fast-transport-belt");
                belt.itemPositions[0].push_back(BeltItemData{ id, 0 });
                belt.itemPositions[1].push_back(BeltItemData{ id, 0 });
                Coordinator::Instance().AddComponent(toadd, belt);
                done = 1;
            } else {
                BeltComponent belt{
                    {
                    }
                };
                belt.isCorner = args["iscorner"].get_or(false),
                belt.cornerFromDir = args["direction1"].get_or(Direction::North),
                 Coordinator::Instance().AddComponent(toadd, belt);
                 std::cout << "[Surface] Creating belt: " << name << " at (" << x << ", " << y << ") direction: " << dir << "isCorner: " << belt.isCorner << "belt.cornerToDir: " << belt.cornerFromDir << std::endl;
            }
    }
    if(name=="fast-inserter") {
        InserterComponent inserter{};
        Coordinator::Instance().AddComponent(toadd, inserter);
    }
    chunks[getTileChunkCoord(x, y).chunk]->addEntity(toadd);

}

  struct NormalColorVertex {
    float x, y;
    float u, v;
};

void Surface::update(double dt) {
//    for (auto& entity : entities) {
  //      entity->update(dt);
//    }
}

std::optional<Entity> Surface::FindEntityByCoords(int64_t x, int64_t y) {
    struct TileChunkCoord coords = getTileChunkCoord(x, y);
    const std::vector<Entity>& entities = chunks[coords.chunk]->getEntityList();
    for(Entity entity : entities) {
        const Vec2& pos = Coordinator::Instance().GetComponent<Vec2>(entity);
        if(pos.x == x && pos.y == y) {
            return entity;
        }
    }
    return std::nullopt;
}

void Surface::draw(bgfx::VertexBufferHandle vbo, bgfx::IndexBufferHandle ibo, bgfx::ProgramHandle program) {
    statate_to_render(camera, ttm, renderQueue, this);
    const auto& queue = renderQueue.getFrontList();

    for (const RenderObject& ro : queue)
    {
        float model[16];
        bx::mtxSRT(model,
                   ro.w, ro.h, 1.0f,
                   0.0f, 0.0f, ro.rotation,
                   ro.x, ro.y, 0.0f);

        float uvRect[4] = { ro.tex.u0, ro.tex.v0, ro.tex.u1, ro.tex.v1 };
        bgfx::setUniform(u_uvRectHandle, uvRect);
        bgfx::setTexture(0, s_texColorUniform, TextureAtlasSystem::getInstance().getAtlasTexture(ro.tex.atlasId));
        bgfx::setTransform(model);
        bgfx::setVertexBuffer(0, vbo);
        bgfx::setIndexBuffer(ibo);
        bgfx::setState(ro.state);
        bgfx::submit(0, program);
    }

    bgfx::frame();
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
