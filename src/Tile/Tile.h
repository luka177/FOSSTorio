#pragma once

struct TileCoord {
    int tileX;
    int tileY;
};

class Tile {
    public:
        Tile(int type);
        Tile();
        int get_type();
        int getTextureId();
        int getTexture();
        void set_type(int type);

    private:
        int type;
        int texture;
        int texture_id;
};
