#pragma once

#include <vector>

#include "Vector2.h"
#include "Path.h"
#include "Triangle.h"

class Tesselator
{
public:
    static void Stroke(const Path& path, std::vector<Triangle>& triangles, float width, float miterLimit = 10.0f);
    static void Fill(const Path& path, std::vector<Triangle>& triangles);
};