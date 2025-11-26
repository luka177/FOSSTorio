#pragma once
#include <iostream>

#include <sol/sol.hpp>

void printSolObject(const sol::object& obj, int indent = 0);
void printSolTable(const sol::table& tbl, int indent = 0);
std::string resolveLuaPath(const std::string& path);