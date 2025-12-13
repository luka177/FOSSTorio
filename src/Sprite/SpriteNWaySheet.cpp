#include <Sprite/SpriteNWaySheet.h>

SpriteNWaySheet::SpriteNWaySheet(sol::table sprite)
    : SpriteParmeters(sprite) {
    frames = sprite["frames"].get_or(4);
}