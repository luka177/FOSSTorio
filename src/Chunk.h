#pragma once

#include <cstddef>
#include <functional>

#include <Tile/Tile.h>
#include <Entity/EntityManager.h>

const int CHUNK_SIZE = 32;

struct ChunkCoord {
    int chunkX;
    int chunkY;
};

inline bool operator==(const ChunkCoord& a, const ChunkCoord& b) noexcept {
    return a.chunkX == b.chunkX && a.chunkY == b.chunkY;
}

namespace std {
    template<>
    struct hash<ChunkCoord> {
        std::size_t operator()(ChunkCoord const& c) const noexcept {
            std::size_t h1 = std::hash<int>{}(c.chunkX);
            std::size_t h2 = std::hash<int>{}(c.chunkY);
            return h1 ^ (h2 + 0x9e3779b9u + (h1 << 6) + (h1 >> 2));
        }
    };
}

struct ChunkCoordHash {
    std::size_t operator()(const ChunkCoord& c) const {
        return std::hash<int>()(c.chunkX) ^ (std::hash<int>()(c.chunkY) << 1);
    }
};

class Chunk {
public:
    Chunk();
    // Do we want to check coords?
    void addEntity(Entity entity) { entity_list.push_back(entity); }
    const std::vector<Entity>& getEntityList() const { return entity_list; }
    void removeEntity(Entity entity);
    Tile& getTile(int localX, int localY);

private:
    std::vector <Entity> entity_list;
// TBD: NO, WTF WAS I THINKING??? WHY WOULD I STORE EACH TILE, MAKE IT OPTIONAL NAD BASE ON SEED IF NONE
    Tile tiles[CHUNK_SIZE][CHUNK_SIZE];
};
