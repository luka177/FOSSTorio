#include <Lua/LuaHelper.h>

void printSolTable(const sol::table& tbl, int indent) {
    std::string pad(indent, ' ');
    for (auto& kv : tbl) {
        std::string key;

        if (kv.first.get_type() == sol::type::string) {
            key = kv.first.as<std::string>();
        } else if (kv.first.get_type() == sol::type::number) {
            key = std::to_string(kv.first.as<int>());
        } else {
            key = "<non-printable-key>";
        }

        sol::object value = kv.second;
        std::cout << pad << key << " = ";
        printSolObject(value, indent + 2);
    }
}

void printSolObject(const sol::object& obj, int indent) {
    switch (obj.get_type()) {
        case sol::type::string:
            std::cout << obj.as<std::string>() << "\n";
            break;

        case sol::type::number:
            std::cout << obj.as<double>() << "\n";
            break;

        case sol::type::boolean:
            std::cout << (obj.as<bool>() ? "true" : "false") << "\n";
            break;

        case sol::type::table:
            std::cout << "{\n";
            printSolTable(obj.as<sol::table>(), indent + 2);
            std::cout << std::string(indent, ' ') << "}\n";
            break;

        default:
            std::cout << "<" << sol::type_name(obj.lua_state(), obj.get_type()) << ">\n";
            break;
    }
}

std::string resolveLuaPath(const std::string& path) {
    if (path.compare(0, 9, "__base__/") == 0) {
        return "data/base/" + path.substr(9);
    } else if (path.compare(0, 9, "__core__/") == 0) {
        return "data/core/" + path.substr(9);
    } else if (path.compare(0, 2, "__") == 0) {
        size_t secondUnderscore = path.find("__", 2);
        if (secondUnderscore != std::string::npos) {
            std::string mod = path.substr(2, secondUnderscore - 2);
            std::string rest = path.substr(secondUnderscore + 2);
            return "data/" + mod + "/" + rest;
        }
    }
    return path;
}