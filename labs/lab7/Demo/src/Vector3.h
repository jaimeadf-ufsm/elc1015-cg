#pragma once

#include <cmath>

struct Vector3
{
    float X;
    float Y;
    float Z;

    Vector3(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {}

    float Length() const
    {
        return std::sqrt(X * X + Y * Y + Z * Z);
    }

    Vector3& operator*=(float scalar)
    {
        X *= scalar;
        Y *= scalar;
        Z *= scalar;
        return *this;
    }
};