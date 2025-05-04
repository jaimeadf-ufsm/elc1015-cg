#pragma once

#include "Vector2.h"

struct Triangle
{
    Vector2 A;
    Vector2 B;
    Vector2 C;

    Triangle();
    Triangle(const Vector2& a, const Vector2& b, const Vector2& c);

    bool Contains(const Vector2& point) const;
};