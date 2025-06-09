#pragma once

#include <cmath>
#include "Vector2.h"

#define MATH_PI 3.14159265358979323846f
#define MATH_TAU (2 * MATH_PI)

template<typename T>
T Lerp(const T& a, const T& b, float t) {
    return a + (b - a) * t;
}

template<typename T>
T Clamp(const T& value, const T& min, const T& max) {
    return (value < min) ? min : (value > max) ? max : value;
}

bool IsPointInRectangle(const Vector2& point, const Vector2& position, const Vector2& size);
bool IsPointInCircle(const Vector2& point, const Vector2& center, float radius);
