#include "Triangle.h"

#include <cstdio> // For printf

Triangle::Triangle() : A(), B(), C() {}

Triangle::Triangle(const Vector2& a, const Vector2& b, const Vector2& c)
    : A(a), B(b), C(c) {}

bool Triangle::Contains(const Vector2& point) const
{
    Vector2 ab = B - A;
    Vector2 ac = C - A;

    Vector2 ap = point - A;

    float d = ab.X * ac.Y - ab.Y * ac.X;

    float u = ap.X * ac.Y - ap.Y * ac.X;
    float v = ab.X * ap.Y - ab.Y * ap.X;

    return (u >= 0.0f) && (v >= 0.0f) && (u + v <= d);
}