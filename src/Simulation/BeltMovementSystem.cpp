#include <Simulation/BeltMovementSystem.h>
#include <Entity/Coordinator.h>

//TBD: read from prototype?
constexpr int TICK_MOVE_POSITIONS = 8;

bool IsRightTurn(Direction from, Direction to)
{
    switch (from)
    {
        case Direction::North: return to == Direction::East;
        case Direction::East:  return to == Direction::South;
        case Direction::South: return to == Direction::West;
        case Direction::West:  return to == Direction::North;
    }
    return false;
}

bool IsCorner(Direction from, Direction to)
{
    if (from == to) return false;
    return true;
}

bool IsInnerLane(int laneIndex, Direction from, Direction to)
{
    // No inner/outer lanes on corners
    if (!IsCorner(from, to))
        return false;

    const bool rightTurn = IsRightTurn(from, to);

    if (rightTurn)
        return laneIndex == 1;
    else
        return laneIndex == 0;
}

int PositionsPerLaneForTile(const BeltComponent& beltComp, size_t lane, Direction dir)
{
    if (!beltComp.isCorner)
        return STRAIGHT_POSITIONS_PER_LANE;

    const bool inner = IsInnerLane(lane, beltComp.cornerFromDir, dir);
    return inner ? CORNER_INNER_POSITIONS_PER_LANE
                 : CORNER_OUTER_POSITIONS_PER_LANE;
}

int GetPositionsPerLaneForTile(int laneIndex,
                               const BeltComponent& beltComp,
                               Direction beltDir)
{
    if (!beltComp.isCorner)
        return STRAIGHT_POSITIONS_PER_LANE;

    const Direction from = beltComp.cornerFromDir;

    const bool inner = IsInnerLane(laneIndex, from, beltDir);
    return inner ? CORNER_INNER_POSITIONS_PER_LANE
                 : CORNER_OUTER_POSITIONS_PER_LANE;
}

void BeltMovementSystem::Update()
{
    auto& beltComponents =  Coordinator::Instance().GetComponentArrayRef<BeltComponent>();

    for (size_t i = 0; i < beltComponents.Size(); ++i)
    {
        Entity beltEntity = beltComponents.GetEntityByIndex(i);
        BeltComponent& beltComp = beltComponents.GetComponentByIndex(i);

        if ( Coordinator::Instance().HasComponent<Vec2>(beltEntity))
        {
            Vec2& pos = Coordinator::Instance().GetComponent<Vec2>(beltEntity);
            Direction& dir = Coordinator::Instance().GetComponent<Direction>(beltEntity);
            ProcessBeltTile(beltEntity, pos, dir, beltComp);
        }
    }
}

void BeltMovementSystem::ProcessBeltTile(Entity beltEntity, Vec2 pos, Direction dir, BeltComponent& beltComp)
{
    for (int lane = 0; lane < LANES_PER_BELT; ++lane)
    {
        PushItemToNextTile(lane, beltComp, dir, pos);
        MoveItemsInLane(lane, beltComp, dir, pos);
    }
}

void BeltMovementSystem::MoveItemsInLane(int laneIndex, BeltComponent& beltComp, Direction beltDir, Vec2 pos)
{
     const int positionsPerLane =
        GetPositionsPerLaneForTile(laneIndex, beltComp, beltDir);

    for (auto it = beltComp.itemPositions[laneIndex].rbegin();
         it != beltComp.itemPositions[laneIndex].rend(); ++it)
    {
        if (it->itemPrototype.has_value())
        {
            int new_pos = it->start_pos + TICK_MOVE_POSITIONS;
            if (std::next(it) != beltComp.itemPositions[laneIndex].rend())
            {
                const auto& itemInFront = *std::next(it);
                int max_allowed_pos = itemInFront.start_pos - ITEM_POSITION_LENGTH;
                new_pos = std::min(new_pos, max_allowed_pos);
            }
            if (new_pos < positionsPerLane)
            {
                it->start_pos = new_pos;
            }
        }
    }
}


void BeltMovementSystem::PushItemToNextTile(int laneIndex, BeltComponent& currentBeltComp, Direction beltDir, Vec2 pos)
{
    if (currentBeltComp.itemPositions[laneIndex].empty()) return;
    auto& exitingItemData = currentBeltComp.itemPositions[laneIndex].back();
    const int positionsPerLane =
        GetPositionsPerLaneForTile(laneIndex, currentBeltComp, beltDir);
    if (exitingItemData.start_pos + TICK_MOVE_POSITIONS >= positionsPerLane)
    {
        std::optional<Entity> nextBeltEntity = GetNextBeltEntity(pos, beltDir);
        if (nextBeltEntity.has_value())
        {
            BeltComponent& nextBeltComp = Coordinator::Instance().GetComponent<BeltComponent>(nextBeltEntity.value());
            int entry_pos = 0;
            if (!nextBeltComp.itemPositions[laneIndex].empty())
            {
                const auto& firstItem = nextBeltComp.itemPositions[laneIndex].front();
                if (firstItem.start_pos < ITEM_POSITION_LENGTH)
                {
                    return;
                }
            }

            BeltItemData newItemData;
            newItemData.itemPrototype = exitingItemData.itemPrototype;
            newItemData.start_pos = entry_pos;
            nextBeltComp.itemPositions[laneIndex].insert(
                nextBeltComp.itemPositions[laneIndex].begin(),
                newItemData
            );

            currentBeltComp.itemPositions[laneIndex].pop_back();
        }
        else
        {
            currentBeltComp.itemPositions[laneIndex].pop_back();
            //DestroyEntity(exitingItemData.itemPrototype.value());
        }
    }
}

Vec2 CalculateNextTilePosition(const Vec2& currentTilePos, Direction dir)
{
    Vec2 nextPos = currentTilePos;

    switch (dir)
    {
        case Direction::North:
            nextPos.y += 1;
            break;
        case Direction::East:
            nextPos.x += 1;
            break;
        case Direction::South:
            nextPos.y -= 1;
            break;
        case Direction::West:
            nextPos.x -= 1;
            break;
    }
    return nextPos;
}

std::optional<Entity> BeltMovementSystem::GetNextBeltEntity(Vec2 currentTile, Direction beltDir)
{
    auto& coord = Coordinator::Instance();

    Vec2 targetPos = CalculateNextTilePosition(currentTile, beltDir);
    auto& beltComponents = coord.GetComponentArrayRef<BeltComponent>();

    for (size_t i = 0; i < beltComponents.Size(); ++i)
    {
        Entity entityID = beltComponents.GetEntityByIndex(i);
        const Vec2& potentialPos = coord.GetComponent<Vec2>(entityID);

        constexpr float EPSILON = 0.001f;

        if (std::abs(potentialPos.x - targetPos.x) < EPSILON &&
            std::abs(potentialPos.y - targetPos.y) < EPSILON)
        {
            return entityID;
        }
    }

    return std::nullopt;
}