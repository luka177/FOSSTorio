#include <cstdlib>

#include <Tile/Tile.h>

Tile::Tile(int type)
    : type(type)
{

}

Tile::Tile() : type(0)
{
    texture_id = rand() % 15;
    texture = rand() % 50;
}

int Tile::get_type() {
    return type;
}

int Tile::getTextureId() {
    return texture_id;
}

int Tile::getTexture() {
    return texture;
}

void Tile::set_type(int mtype){
    type = mtype;
}
