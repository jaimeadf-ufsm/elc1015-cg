#pragma once

#include <cmath>

struct Vector2
{
    float X;
    float Y;

    Vector2(float x = 0, float y = 0) : X(x), Y(y) {}

    float Length() const
    {
        return std::sqrt(X * X + Y * Y);
    }

    Vector2& operator*=(float scalar)
    {
        X *= scalar;
        Y *= scalar;
        return *this;
    }

    Vector2& operator+=(const Vector2& other)
    {
        X += other.X;
        Y += other.Y;
        return *this;
    }

    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(X + other.X, Y + other.Y);
    }

    Vector2 operator-(const Vector2& other) const
    {
        return Vector2(X - other.X, Y - other.Y);
    }

    static Vector2 Normalize(const Vector2& v)
    {
        float length = v.Length();

        return Vector2(v.X / length, v.Y / length);
    }

    static Vector2 Rotate(const Vector2& v, float angle)
    {
        float cosAngle = std::cos(angle);
        float sinAngle = std::sin(angle);

        return Vector2(
            v.X * cosAngle - v.Y * sinAngle,
            v.X * sinAngle + v.Y * cosAngle
        );
    }
};