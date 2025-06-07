#pragma once

#include "Vector2.h"

template<typename Iterator>
Iterator Lerp(const Iterator& a, const Iterator& b, float t) {
    return a + (b - a) * t;
}

bool IsPointInRectangle(const Vector2& point, const Vector2& position, const Vector2& size);
bool IsPointInCircle(const Vector2& point, const Vector2& center, float radius);
