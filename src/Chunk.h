#pragma once

#include <cstddef>
#include <functional>

#include <Tile/Tile.h>

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

    Tile tiles[CHUNK_SIZE][CHUNK_SIZE];

    Tile& getTile(int localX, int localY);
};
