#pragma once

#include <cstddef>
#include <functional>

#include <Tile/Tile.h>
#include <Entity/EntityRegister.h>

const int CHUNK_SIZE = 32;

struct ChunkCoord {
    int chunkX;
    int chunkY;

    bool operator==(const ChunkCoord& other) const {
        return chunkX == other.chunkX && chunkY == other.chunkY;
    }
};

struct ChunkCoordHash {
    std::size_t operator()(const ChunkCoord& c) const {
        return std::hash<int>()(c.chunkX) ^ (std::hash<int>()(c.chunkY) << 1);
    }
};

class Chunk {
public:
    Chunk();
    // Do we want to check coords?
    void addEntity(EntityID entity) { entity_list.push_back(entity); }
    const std::vector<EntityID>& getEntityList() const { return entity_list; }
    void removeEntity(EntityID entity);
    Tile& getTile(int localX, int localY);

private:
    std::vector <EntityID> entity_list;
// TBD: NO, WTF WAS I THINKING??? WHY WOULD I STORE EACH TILE, MAKE IT OPTIONAL NAD BASE ON SEED IF NONE
    Tile tiles[CHUNK_SIZE][CHUNK_SIZE];
};
