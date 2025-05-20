#pragma once

#include "DrawingContext.h"

class PrimitiveGraphic
{
public:
    static void FillMesh(DrawingContext& context, const Mesh& mesh);
    static void FillCircle(DrawingContext& context, Vector2 center, float radius);
};