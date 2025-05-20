#include "Bezier.h"

Vector2 QuadraticBezier(const Vector2& p0, const Vector2& c1, const Vector2& p1, float t)
{
    Vector2 p;
    p += (1 - t) * (1 - t) * p0;
    p += 2 * (1 - t) * t * c1;
    p += t * t * p1;

    return p;
}

Vector2 CubicBezier(const Vector2& p0, const Vector2& c1, const Vector2& c2, const Vector2& p1, float t)
{
    Vector2 p;
    p += (1 - t) * (1 - t) * (1 - t) * p0;
    p += 3 * (1 - t) * (1 - t) * t * c1;
    p += 3 * (1 - t) * t * t * c2;
    p += t * t * t * p1;

    return p;
}