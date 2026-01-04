#pragma once
#include <ostream>
#include <optional>
#include <vector>
#include <array>

#include <misc.h>
#include <Entity/EntityManager.h>
#include <Tile/Tile.h>

#include <Prototype/PrototypeRegister.h>

//TBD: we need items register, items can have quality and health
struct InserterComponent
{
    double rotation;
    double extension;
    std::optional<PrototypeID> itemPrototype;
    uint8_t itemCount;
    Vec2d target;
    bool moveStarted;
};

class InserterSystem
{
public:
    InserterSystem() {}

    void Update();
private:
    void move(Vec2& pos, Direction dir, Vec2d& dst, double maxMove, double maxRotate, InserterComponent& inserterComp);
    void ProcessInserterEntity(Entity entity, Vec2& pos, Direction& dir, InserterComponent& inserterComp);
};