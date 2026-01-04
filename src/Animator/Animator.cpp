#include <vector>
#include <Camera/Camera.h>
#include <Renderer/RenderQueue.h>
#include <Atlas/TextureAtlasSystem.h>
#include <Entity/Coordinator.h>
#include <EntitiesPrototypes/CraftingMachinePrototype.h>
#include <EntitiesPrototypes/TransportBeltConnectablePrototype.h>
#include <EntitiesPrototypes/InserterPrototype.h>
#include <Animation/AnimationParameters.h>
#include <Tile/TileTextureManager.h>
#include <Surface/Surface.h>
#include <Simulation/BeltMovementSystem.h>
#include <Simulation/InserterSystem.h>
#include <misc.h>

const float tileSize = 32.0f;

void tilesToRender(TileTextureManager *ttm, std::vector<RenderObject>& queue, Surface *surface, int minTileX, int maxTileX, int minTileY, int maxTileY) {
    const auto& chunks = surface->getChunks();
    for (int y = minTileY; y <= maxTileY; ++y) {
        for (int x = minTileX; x <= maxTileX; ++x) {
            // Convert global tile coord to chunk/tile-in-chunk
            TileChunkCoord coord = surface->getTileChunkCoord(x, y);
            ChunkCoord chunkCoord = coord.chunk;

            auto it = chunks.find(chunkCoord);
            if (it == chunks.end()) {
                continue; // Chunk doesn't exist, skip
            }

            Chunk* chunk = it->second.get();
            Tile& tile = chunk->getTile(coord.tile.tileX, coord.tile.tileY);

            RenderObject ro;
            //TBD: Dont assume grass-1
            if((x + y) % 2 == 0) {
                ro.tex = ttm->getTileUVInfo("grass-1", 1, tile.getTextureId(), AtlasTag{});
            } else {
                ro.tex = ttm->getTileUVInfo("dirt-1", 1, tile.getTextureId(), AtlasTag{});
            }
            ro.x = x * tileSize;
            ro.y = y * tileSize;
            ro.w = tileSize;
            ro.h = tileSize;
            ro.rotation = 0;
            ro.state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A;
            //TBD: Dont assume GroundLayer1, as we have multi-layer tiles for smooth tile types transition
            ro.layer = RenderLayer::GroundLayer1;
            queue.push_back(ro);
        }
    }
}

void craftingMachineToRender(std::vector<RenderObject>& queue, const CraftingMachinePrototype* machine, const Vec2& pos, Entity entity) {
    if(machine->get_graphics_set()->getAnimation()->getAnimation()->getLayersCount()!=0) {
        for(Animation anim : machine->get_graphics_set()->getAnimation()->getAnimation()->getLayers()){
            RenderObject ro;
            ro.tex = TextureAtlasSystem::getInstance().getUV(anim.getSprite());
            ro.x = pos.x * tileSize;
            ro.y = pos.y * tileSize;
            ro.w = tileSize;
            ro.h = tileSize;
            ro.rotation = 0;
            // IS it alvays object?
            ro.layer = RenderLayer::Object;
            if(!anim.getDrawAsShadow()) {
                ro.state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_NORMAL;
                queue.push_back(ro);
            } else {
                // HACK: Dont queue shadows for now
                ro.state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_NORMAL;
            }
        }
    }
}

void ComputeItemWorldPosition(const Vec2& tilePos,
                              const BeltComponent& beltComp,
                              Direction toDir,
                              std::size_t lane,
                              uint16_t startPos,
                              float tileSize,
                              float& worldX,
                              float& worldY)
{
    Direction fromDir = beltComp.cornerFromDir;

    float halfTile = 0.5f * tileSize;
    float centerX = tilePos.x * tileSize + halfTile;
    float centerY = tilePos.y * tileSize + halfTile;

    if (!beltComp.isCorner)
    {
        float offset    = (startPos / float(STRAIGHT_POSITIONS_PER_LANE)) * tileSize;
        float laneOffset = tileSize * 0.35f;
        float side       = (lane == 0) ? -laneOffset : laneOffset;

        Vec2 dir = dirToVec(toDir);
        worldX = centerX + dir.x * (offset - halfTile);
        worldY = centerY + dir.y * (offset - halfTile);
        float perpX = -dir.y;
        float perpY = dir.x;

        worldX += perpX * side;
        worldY += perpY * side;
        worldX -= 16;
        // HACK: Center pos changed, just offset for now
        worldY -= 8;
        return;
    }

    bool inner = IsInnerLane(lane, fromDir, toDir);

    const int positionsPerLane =
        inner ? CORNER_INNER_POSITIONS_PER_LANE : CORNER_OUTER_POSITIONS_PER_LANE;

    float t = startPos / float(positionsPerLane);


    Vec2 vTo   = dirToVec(toDir);
    Vec2 vFrom = dirToVec(fromDir);
    // Decide for circle center, it is alvays in a corner, but what corner, using reletive from vector is relevant here (NOT A BUG)
    if(vFrom.x == 0) {
        centerY = (tilePos.y+(0.5f*vFrom.y)) * tileSize;
        centerX = (tilePos.x+(0.5f*vTo.x)) * tileSize;
    } else {
        centerY = (tilePos.y+(0.5f*vTo.y)) * tileSize;
        centerX = (tilePos.x+(0.5f*vFrom.x)) * tileSize;
    }
    // HACK: Center pos changed, just offset for now
    centerY += 8;
    // Different radius for inner vs outer lane
    float innerRadius = tileSize * 0.15f;
    float outerRadius = tileSize * 0.85f;
    float radius      = inner ? innerRadius :  outerRadius;

    float angleFromDir = directionToAngle(fromDir);
    // FROM unlike TO is RELETIVE to this tile not compass accurate thus west means object is moving EAST towars positive X, and enters from west
    vFrom.x = -vFrom.x;
    vFrom.y = -vFrom.y;

    float cross = vFrom.x * vTo.y - vFrom.y * vTo.x;
    float sign = (cross > 0.0f) ? 1.0f : -1.0f;
    float delta = sign * (0.5f * PI);

    float angleStart = angleFromDir + sign * (0.5f * PI);
    float angle      = angleStart + t * delta;

    float localX = std::cos(angle) * radius;
    float localY = std::sin(angle) * radius;

    worldX = centerX + localX;
    worldY = centerY + localY;
}

void transportBeltToRender(std::vector<RenderObject>& queue, const TransportBeltConnectablePrototype* belt, const Vec2& pos, Entity entity) {
    RenderObject ro;
    if(belt->get_animation_set()->getAnimation()->getLayersCount()!=0) {
        //TBD: This is very lacking compared to single layer impl
        for(RotatedAnimation anim : belt->get_animation_set()->getAnimation()->getLayers()){
            const struct AtlasUV& uv = TextureAtlasSystem::getInstance().getUV(anim.getSprite());
            ro.tex = TextureAtlasSystem::getInstance().getUV(anim.getSprite());
            ro.x = pos.x * tileSize;
            ro.y = pos.y * tileSize - tileSize * 0.5f;
            ro.w = anim.getWidth();
            ro.h = anim.getHeight();
            ro.rotation = 0;
            ro.layer = RenderLayer::TransportBelt;
            ro.state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_NORMAL;
            queue.push_back(ro);
        }
    } else {
        uint8_t dir;
        // TBD: is single direction belt texture in this context a thing?
        if(Coordinator::Instance().GetComponent<BeltComponent>(entity).isCorner) {
            dir = belt->get_animation_set()->cornerDirectionToDirectionID(Coordinator::Instance().GetComponent<BeltComponent>(entity).cornerFromDir, Coordinator::Instance().GetComponent<Direction>(entity)) - 1;
        } else {
            dir = belt->get_animation_set()->directionToDirectionID(Coordinator::Instance().GetComponent<Direction>(entity)) - 1;
        }
        AnimationFrameComponent &frameid = Coordinator::Instance().GetComponent<AnimationFrameComponent>(entity);
        // Initial frame would be set to 0, adjust depending on belt direction
        if(frameid.frame < dir*(belt->get_animation_set()->getAnimation()->getFrameCount())) {
                frameid.frame = dir*(belt->get_animation_set()->getAnimation()->getFrameCount());
        }

        ro.tex = TextureAtlasSystem::getInstance().getUV(belt->get_animation_set()->getAnimation()->getSprites()[frameid.frame]);
        //TBD: figure if this is the right place to tick animation (its not representing real state thus isnt relevant to engine determinism)
        if(frameid.frame == ((dir + 1) *(belt->get_animation_set()->getAnimation()->getFrameCount())) - 1) {
                frameid.frame = dir*(belt->get_animation_set()->getAnimation()->getFrameCount());
        } else {
            frameid.frame++;
        }

        ro.x = pos.x * tileSize;
        ro.y = pos.y * tileSize - tileSize * 0.5f;
        //TBD: dont hardcode 64x64?
        ro.w = 64;
        ro.h = 64;
        ro.rotation = 0;
        ro.state =  BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_NORMAL;
        ro.layer = RenderLayer::TransportBelt;
        queue.push_back(ro);
    }

    for (std::size_t lane = 0; lane < 2; ++lane)
    {
        auto& laneVec = Coordinator::Instance().GetComponent<BeltComponent>(entity).itemPositions[lane];

        for (auto& item : laneVec)
        {
            if (item.itemPrototype) {
                ro.tex = TextureAtlasSystem::getInstance().getUV((dynamic_cast<const TransportBeltConnectablePrototype*>(PrototypeRegister::getInstance().GetPrototypeByID(item.itemPrototype.value()))->getIcon()));

                Direction dir = Coordinator::Instance().GetComponent<Direction>(entity);
                float offset = (item.start_pos / float(256)) * tileSize;

                float worldX, worldY;
                ComputeItemWorldPosition(pos, Coordinator::Instance().GetComponent<BeltComponent>(entity), Coordinator::Instance().GetComponent<Direction>(entity), lane, item.start_pos, tileSize, worldX, worldY);

                ro.x = worldX;
                ro.y = worldY;
                ro.w = 16;
                ro.h = 16;
                ro.rotation = 0;
                ro.layer = RenderLayer::Item;
                ro.state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_NORMAL;
                queue.push_back(ro);
            }
        }
    }
}

static Vec2f rotate(Vec2f v, float rad) {
    float c = cosf(rad), s = sinf(rad);
    return { v.x * c - v.y * s, v.x * s + v.y * c };
}

void inserterToRender(std::vector<RenderObject>& queue,
                      const InserterPrototype* inserter,
                      const Vec2& pos,
                      Entity entity)
{
    const auto& ic = Coordinator::Instance().GetComponent<InserterComponent>(entity);;

    RenderObject ro;
     float rot = float(PI * 0.5) - float(ic.rotation * 2.0 * PI);
     if (rot < 0.0f) rot += float(2.0 * PI);
    std::cout << "inserterToRender rot: " << rot << " ic.rotation: " << ic.rotation << "\n";
    // Base
    ro.tex = TextureAtlasSystem::getInstance().getUV(
        inserter->get_hand_base_picture().value().getSprite()
    );
    // base is centered on tile
    ro.w = 8;
    ro.h = 34;
    ro.x = pos.x * tileSize;
    ro.y = pos.y * tileSize + tileSize*0.5f;
    ro.rotation = rot;
    ro.layer = RenderLayer::ItemInInserterHand;
    ro.state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_NORMAL;
    queue.push_back(ro);

    // Hand
    ro.tex = TextureAtlasSystem::getInstance().getUV(
        inserter->get_hand_open_picture().value().getSprite()
    );

    ro.x = (pos.x * tileSize) + (std::sin(rot) * 34);
    ro.y = (pos.y * tileSize + tileSize*0.5f) + (std::cos(rot) * 34);
    ro.w = 10;
    ro.h = 41;
    ro.rotation = rot;
    ro.layer = RenderLayer::ItemInInserterHand;
    ro.state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_NORMAL;
    queue.push_back(ro);
}

void statate_to_render(Camera *camera, TileTextureManager *ttm, RenderQueue& renderQueue, Surface *surface) {
    const float tileSize = 32.0f;

    float camX = camera->getX();
    float camY = camera->getY();
    float zoom = camera->getZoom();

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

    std::vector<RenderObject>& queue = renderQueue.getBackList();
    queue.clear();
    tilesToRender(ttm, queue, surface, minTileX, maxTileX, minTileY, maxTileY);

    std::unordered_set<ChunkCoord> visibleChunks;
    for (int y = minTileY; y <= maxTileY; ++y) {
        for (int x = minTileX; x <= maxTileX; ++x) {
            TileChunkCoord coord = surface->getTileChunkCoord(x, y);
            visibleChunks.insert(coord.chunk);
        }
    }
    const auto& chunks = surface->getChunks();
    for (const ChunkCoord& chunkCoord : visibleChunks) {
        auto it = chunks.find(chunkCoord);
        if (it == chunks.end()) continue;

        Chunk* chunk = it->second.get();
        for (Entity entity : chunk->getEntityList()) {
            const Vec2& pos = Coordinator::Instance().GetComponent<Vec2>(entity);
            PrototypeID protoId = Coordinator::Instance().GetComponent<PrototypeID>(entity);
            const PrototypeBase *proto = PrototypeRegister::getInstance().GetPrototypeByID(protoId);
            if (const CraftingMachinePrototype* machine = dynamic_cast<const CraftingMachinePrototype*>(proto)) {
                craftingMachineToRender(queue, machine, pos, entity);
            } else if (const TransportBeltConnectablePrototype* belt = dynamic_cast<const TransportBeltConnectablePrototype*>(proto)) {
                transportBeltToRender(queue, belt, pos, entity);
            } else if (const InserterPrototype* inserter = dynamic_cast<const InserterPrototype*>(proto)) {
                inserterToRender(queue, inserter, pos, entity);
            }
        }
    }

    std::sort(queue.begin(), queue.end(),
              [](const RenderObject& a, const RenderObject& b)
              {
                  return static_cast<int>(a.layer) < static_cast<int>(b.layer);
              });
    renderQueue.swapBuffers();
}