#include <Animation/Animation4Way.h>
#include <Lua/LuaHelper.h>

Animation4Way::Animation4Way(sol::table animation4way) {
    std::cout << "[Animation4Way] Dumping table:\n";
    printSolTable(animation4way);
    std::cout << "---- END Animation4Way ----\n";
    if(animation4way["north"].valid()) {
        north = Animation(animation4way["north"].get<sol::table>());
    } else {
        north = Animation(animation4way);
     //   throw std::runtime_error("[Animation4Way] north animation isnt optional, but is missing!\n");
    }
    if(animation4way["north_east"].valid()) {
        north_east = Animation(animation4way["north_east"].get<sol::table>());
    }
    if(animation4way["east"].valid()) {
        east = Animation(animation4way["east"].get<sol::table>());
    }
    if(animation4way["south_east"].valid()) {
        south_east = Animation(animation4way["south_east"].get<sol::table>());
    }
    if(animation4way["south"].valid()) {
        south = Animation(animation4way["south"].get<sol::table>());
    }
    if(animation4way["south_west"].valid()) {
        south_west = Animation(animation4way["south_west"].get<sol::table>());
    }
    if(animation4way["west"].valid()) {
        west = Animation(animation4way["west"].get<sol::table>());
    }
    if(animation4way["north_west"].valid()) {
        north_west = Animation(animation4way["north_west"].get<sol::table>());
    }
}