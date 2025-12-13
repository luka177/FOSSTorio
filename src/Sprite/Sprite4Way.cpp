#include <Sprite/Sprite4Way.h>
#include <Lua/LuaHelper.h>

Sprite4Way::Sprite4Way(sol::table sprite) {
    std::cout << "[Sprite] Dumping table:\n";
    printSolTable(sprite);
    std::cout << "---- END Sprite ----\n";
    if(sprite["sheet"].valid()) {
        sheet = SpriteNWaySheet(sprite["sheet"].get<sol::table>());
        return;
    }
    if(sprite["north"].valid()) {
        north = Sprite(sprite["north"].get<sol::table>());
    } else {
        north = Sprite(sprite);
    }
    if(sprite["east"].valid()) {
        east = Sprite(sprite["east"].get<sol::table>());
    }
    if(sprite["south"].valid()) {
        south = Sprite(sprite["south"].get<sol::table>());
    }
    if(sprite["west"].valid()) {
        west = Sprite(sprite["west"].get<sol::table>());
    }
}