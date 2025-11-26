#include <Chunk.h>

Chunk::Chunk() {}

Tile& Chunk::getTile(int localX, int localY) {
    return tiles[localY][localX];
}
