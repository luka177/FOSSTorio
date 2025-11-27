#include <Chunk.h>

Chunk::Chunk() {}

Tile& Chunk::getTile(int localX, int localY) {
    return tiles[localY][localX];
}

void Chunk::removeEntity(EntityID entity) {
    entity_list.erase(std::remove(entity_list.begin(), entity_list.end(), entity), entity_list.end());
}