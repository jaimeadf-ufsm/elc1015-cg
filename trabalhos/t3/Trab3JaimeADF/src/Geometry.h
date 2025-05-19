#pragma once

#include "Vector2.h"

float ComputeTriangleArea(Vector2 a, Vector2 b, Vector2 c);

bool IsPointInCircle(Vector2 point, Vector2 center, float radius);
bool IsPointInTriangle(Vector2 point, Vector2 a, Vector2 b, Vector2 c);