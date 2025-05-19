#pragma once

#include "Path.h"

class PathParser
{
public:
    static std::vector<Path> ParseCommands(const std::string& d);
};