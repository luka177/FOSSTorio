#include <Simulation/InserterSystem.h>
#include <Simulation/BeltMovementSystem.h>
#include <Entity/Coordinator.h>
#include <Surface/Surface.h>
#include <EntitiesPrototypes/InserterPrototype.h>
#include <EntitiesPrototypes/TransportBeltConnectablePrototype.h>

void InserterSystem::move(Vec2& pos, Direction dir, Vec2d& dst, double maxMove, double maxRotate, InserterComponent& inserterComp) {
    double finalExtension = VecToDistance(dst, pos);
    // Our actual pos is center of tile
    Vec2d rotDst = dst-(pos+0.5f);
    double finalRotation = VecToNormalizedAngle(rotDst);
    double delta = finalExtension - inserterComp.extension;
    delta = std::clamp(delta, -maxMove, maxMove);
    inserterComp.extension += delta;
    delta = finalRotation - inserterComp.rotation;
    delta = std::clamp(delta, -maxRotate, maxRotate);
    inserterComp.rotation += delta;
}

void ComputeItemWorldPosition(const Vec2& tilePos,
                              const BeltComponent& beltComp,
                              Direction toDir,
                              std::size_t lane,
                              uint16_t startPos,
                              double& worldX,
                              double& worldY)
{
    Direction fromDir = beltComp.cornerFromDir;

    double centerX = tilePos.x + 0.5f;
    double centerY = tilePos.y + 0.5f;

    if (!beltComp.isCorner)
    {
        double offset    = (startPos / double(STRAIGHT_POSITIONS_PER_LANE));
        double laneOffset = 0.35f;
        double side       = (lane == 0) ? -laneOffset : laneOffset;

        Vec2 dir = dirToVec(toDir);
        worldX = centerX + dir.x * (offset - 0.5f);
        worldY = centerY + dir.y * (offset - 0.5f);
        double perpX = -dir.y;
        double perpY = dir.x;

        worldX += perpX * side;
        worldY += perpY * side;
        worldX -= 0.5f;
        worldY -= 0.5f;
        return;
    }

    bool inner = IsInnerLane(lane, fromDir, toDir);

    const int positionsPerLane =
        inner ? CORNER_INNER_POSITIONS_PER_LANE : CORNER_OUTER_POSITIONS_PER_LANE;

    double t = startPos / double(positionsPerLane);


    Vec2 vTo   = dirToVec(toDir);
    Vec2 vFrom = dirToVec(fromDir);
    // Decide for circle center, it is always in a corner, but what corner, using reletive from vector is relevant here (NOT A BUG)
    if(vFrom.x == 0) {
        centerY = (tilePos.y+(0.5f*vFrom.y));
        centerX = (tilePos.x+(0.5f*vTo.x));
    } else {
        centerY = (tilePos.y+(0.5f*vTo.y));
        centerX = (tilePos.x+(0.5f*vFrom.x));
    }

    // Different radius for inner vs outer lane
    double innerRadius = 0.15f;
    double outerRadius = 0.85f;
    double radius      = inner ? innerRadius :  outerRadius;

    double angleFromDir = directionToAngle(fromDir);
    // FROM unlike TO is RELETIVE to this tile not compass accurate thus west means object is moving EAST towars positive X, and enters from west
    vFrom.x = -vFrom.x;
    vFrom.y = -vFrom.y;

    double cross = vFrom.x * vTo.y - vFrom.y * vTo.x;
    double sign = (cross > 0.0f) ? 1.0f : -1.0f;
    double delta = sign * (0.5f * PI);

    double angleStart = angleFromDir + sign * (0.5f * PI);
    double angle      = angleStart + t * delta;

    double localX = std::cos(angle) * radius;
    double localY = std::sin(angle) * radius;

    worldX = centerX + localX;
    worldY = centerY + localY;
}

void InserterSystem::ProcessInserterEntity(Entity entity, Vec2& pos, Direction& dir, InserterComponent& inserterComp)
{
    //Get src and dst entites (there can be none)
    std::optional<Entity> src;
    std::optional<Entity> dst;
    const InserterPrototype* proto = dynamic_cast<const InserterPrototype*>(PrototypeRegister::getInstance().GetPrototypeByID(Coordinator::Instance().GetComponent<PrototypeID>(entity)));
    Vec2 dstPos(pos+rotateVecToDirection(proto->get_insert_position(), dir));
    Vec2 srcPos(pos+rotateVecToDirection(proto->get_pickup_position(), dir));
    Vec2d srcPosf(pos+rotateVecToDirection(proto->get_pickup_position(), dir));
    src = Coordinator::Instance().GetComponent<Surface*>(entity)->FindEntityByCoords(srcPos);
    dst = Coordinator::Instance().GetComponent<Surface*>(entity)->FindEntityByCoords(dstPos);
    const PrototypeBase *protoSrc = PrototypeRegister::getInstance().GetPrototypeByID(Coordinator::Instance().GetComponent<PrototypeID>(src.value()));
//    const PrototypeBase *protoDst = PrototypeRegister::getInstance().GetPrototypeByID(Coordinator::Instance().GetComponent<PrototypeID>(dst.value()));

    // Now we need to figure what to do,
    // If we have full stack of items (or started move to dst already) we only can move
    // Otherwise figure whats the state and whether we want to get items
    // Hack we dont have stacks yet so > 0 is good enough

 //   if(inserterComp->moveStarted || inserterComp->itemCount> 0) {
 //       move(pos, &dst, proto->extension_speed, proto->rotation_speed, inserterComp);
 //   }

    // Handle belts
    std::cout << "Handle inseter at: " << pos  <<  " src: " << srcPos << " dst: " << dstPos << "\n";
    if(const TransportBeltConnectablePrototype* srcBelt = dynamic_cast<const TransportBeltConnectablePrototype*>(protoSrc)) {
        // I dont like that, but we have to figure actual inserter arm coords
        Vec2d toolhead;
        double angleRad = inserterComp.rotation * 2.0f * PI;
        toolhead.x = pos.x + inserterComp.extension * std::cos(angleRad);
        toolhead.y = pos.y + inserterComp.extension * std::sin(angleRad);
        // I like that even less, but we need closest item
        // TBD: Only check items we need
        BeltComponent& beltComp = Coordinator::Instance().GetComponent<BeltComponent>(src.value());
        double shortestDistance = std::numeric_limits<double>::max();
        double distance;
        Vec2d srcItemPos;
        for (std::size_t lane = 0; lane < 2; ++lane)
        {
            auto& laneVec = beltComp.itemPositions[lane];

            for (auto& item : laneVec)
            {
                if (ItemRegister::getInstance().Get(item.item.value()).prototype.has_value()) {
                    double offset = (item.start_pos / double(256)) * 32;
                    Vec2d itemPos;
                    ComputeItemWorldPosition(srcPos, beltComp, Coordinator::Instance().GetComponent<Direction>(src.value()), lane, item.start_pos, itemPos.x, itemPos.y);
                    distance = VecToDistance(itemPos, toolhead);
                    if(distance < shortestDistance) {
                        shortestDistance = distance;
                        srcItemPos = itemPos;
                    }
                }
            }
        }

        if(shortestDistance != std::numeric_limits<double>::max()) {
            std::cout << "Found item for inserter at: " << pos << " distance to item: " << shortestDistance << " item pos: " << srcItemPos << "\n";
            move(pos, dir, srcItemPos, proto->get_extension_speed(), proto->get_rotation_speed(), inserterComp);
        } else {
            std::cout << "No item found for inserter at: " << pos  << "\n";
            move(pos, dir, srcPosf, proto->get_extension_speed(), proto->get_rotation_speed(), inserterComp);
        }
    }

}

void InserterSystem::Update()
{
    auto& inserterComponents =  Coordinator::Instance().GetComponentArrayRef<InserterComponent>();

    for (size_t i = 0; i < inserterComponents.Size(); ++i)
    {
        Entity inserterEntity = inserterComponents.GetEntityByIndex(i);
        InserterComponent& inserterComp = inserterComponents.GetComponentByIndex(i);

        if ( Coordinator::Instance().HasComponent<Vec2>(inserterEntity))
        {
            Vec2& pos = Coordinator::Instance().GetComponent<Vec2>(inserterEntity);
            Direction& dir = Coordinator::Instance().GetComponent<Direction>(inserterEntity);
            ProcessInserterEntity(inserterEntity, pos, dir, inserterComp);
        }
    }
}