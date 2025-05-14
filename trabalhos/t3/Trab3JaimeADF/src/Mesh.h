#pragma once

#include <vector>

#include "Vector2.h"

struct Mesh
{
    std::vector<Vector2> Vertices;
    std::vector<size_t> Triangles;

    void Clear();
};