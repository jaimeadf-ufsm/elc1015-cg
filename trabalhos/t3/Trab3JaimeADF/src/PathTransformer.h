#pragma once

#include "Path.h"

class PathTransformer
{
public:
    static void Offset(Path& destination, const Path& source, float amount);
};