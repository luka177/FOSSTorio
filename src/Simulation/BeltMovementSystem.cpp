#include <Simulation/BeltMovementSystem.h>
#include <Entity/Coordinator.h>

//TBD: read from prototype?
constexpr int TICK_MOVE_POSITIONS = 1;

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
    for (auto it = beltComp.itemPositions[laneIndex].rbegin();
         it != beltComp.itemPositions[laneIndex].rend(); ++it)
    {
        if (it->itemEntity.has_value())
        {
            int new_pos = it->start_pos + TICK_MOVE_POSITIONS;

            if (std::next(it) != beltComp.itemPositions[laneIndex].rend())
            {
                const auto& itemInFront = *std::next(it);
                int max_allowed_pos = itemInFront.start_pos - ITEM_POSITION_LENGTH;
                new_pos = std::min(new_pos, max_allowed_pos);
            }

            if (new_pos < POSITIONS_PER_LANE)
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

    if (exitingItemData.start_pos + TICK_MOVE_POSITIONS >= POSITIONS_PER_LANE)
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
            newItemData.itemEntity = exitingItemData.itemEntity;
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
            //DestroyEntity(exitingItemData.itemEntity.value());
        }
    }
}