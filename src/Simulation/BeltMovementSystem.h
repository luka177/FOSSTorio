#pragma once
#include <ostream>
#include <optional>
#include <vector>
#include <array>

#include <misc.h>
#include <Entity/EntityManager.h>
#include <Tile/Tile.h>

#include <Prototype/PrototypeRegister.h>

constexpr int POSITIONS_PER_LANE = 256;
constexpr int ITEM_POSITION_LENGTH = 64;
constexpr int LANES_PER_BELT = 2;
//TBD: we need items register, items can have quality and health
struct BeltItemData
{
    std::optional<PrototypeID> itemPrototype;
    int16_t start_pos;
};

struct BeltComponent
{
    std::array<std::vector<BeltItemData>, LANES_PER_BELT> itemPositions;
};

class BeltMovementSystem
{
public:
    BeltMovementSystem() {}

    void Update();

private:
    void ProcessBeltTile(Entity beltEntity, Vec2 pos, Direction dir, BeltComponent& beltComp);
    void MoveItemsInLane(int laneIndex, BeltComponent& beltComp, Direction beltDir, Vec2 tilePos);
    void PushItemToNextTile(int laneIndex, BeltComponent& currentBeltComp, Direction beltDir, Vec2 currentTilePos);
    std::optional<Entity> GetNextBeltEntity(Vec2 currentTile, Direction dir);
};