#pragma once

#include <vector>

#include "Vector2.h"
#include "Polyline.h"
#include "Triangle.h"

class Tesselator
{
public:
    static void Stroke(const PolyLine& polyLine, std::vector<Triangle>& triangles, float width, float miterLimit = 10.0f);
    static void Fill(const PolyLine& polyLine, std::vector<Triangle>& triangles);
};