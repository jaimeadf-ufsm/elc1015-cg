#include "Geometry.h"

float ComputeTriangleArea(Vector2 a, Vector2 b, Vector2 c)
{
    return (b - a).Cross(c - b) / 2.0f;
}

bool IsPointInCircle(Vector2 point, Vector2 center, float radius)
{
    return (point - center).MagnitudeSquared() <= radius * radius;
}

bool IsPointInTriangle(Vector2 point, Vector2 a, Vector2 b, Vector2 c)
{
    Vector2 ab = b - a;
    Vector2 ac = c - a;

    Vector2 ap = point - a;

    float d = ab.X * ac.Y - ab.Y * ac.X;

    float u = ap.X * ac.Y - ap.Y * ac.X;
    float v = ab.X * ap.Y - ab.Y * ap.X;

    return (u >= 0.0f) && (v >= 0.0f) && (u + v <= d);
}