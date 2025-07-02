#pragma once

#include "Mathematics.h"

struct Vector4
{
    float X;
    float Y;
    float Z;
    float W;

    Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f)
        : X(x), Y(y), Z(z), W(w) {}

    const float* ToArray() const
    {
        return &X;
    }
};

struct Vector3
{
    float X;
    float Y;
    float Z;

    Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : X(x), Y(y), Z(z) {}
    
    const float* ToArray() const
    {
        return &X;
    }

    Vector3 operator+(const Vector3& other) const
    {
        return Vector3(X + other.X, Y + other.Y, Z + other.Z);
    }

    Vector3 operator-(const Vector3& other) const
    {
        return Vector3(X - other.X, Y - other.Y, Z - other.Z);
    }

    Vector3 operator*(float scalar) const
    {
        return Vector3(X * scalar, Y * scalar, Z * scalar);
    }

    float Length() const
    {
        return sqrtf(X * X + Y * Y + Z * Z);
    }

    Vector3 Normalize() const
    {
        float len = Length();
        if (len > 0.0f)
            return Vector3(X / len, Y / len, Z / len);
        return Vector3(0, 0, 0);
    }

    float Dot(const Vector3& other) const
    {
        return X * other.X + Y * other.Y + Z * other.Z;
    }

    Vector3 Cross(const Vector3& other) const
    {
        return Vector3(
            Y * other.Z - Z * other.Y,
            Z * other.X - X * other.Z,
            X * other.Y - Y * other.X
        );
    }
};

struct Vector2
{
    float X;
    float Y;

    Vector2(float u = 0.0f, float v = 0.0f)
        : X(u), Y(v) {}
    
    const float* ToArray() const
    {
        return &X;
    }
};