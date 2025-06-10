#pragma once

#include "Vector2.h"

struct ColorRGB;

struct Vector3
{
    float X;
    float Y;
    float Z;

    Vector3();
    Vector3(const ColorRGB& color);
    Vector3(const Vector2& xy, float z);
    Vector3(float x, float y, float z);

    Vector3 Normalize() const;

    float Dot(const Vector3& other) const;
    Vector3 Cross(const Vector3& other) const;

    float Magnitude() const;
    float MagnitudeSquared() const;

    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(const Vector3& other) const;
    Vector3 operator/(const Vector3& other) const;

    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;

    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    Vector3& operator*=(const Vector3& other);
    Vector3& operator/=(const Vector3& other);

    Vector3& operator*=(float scalar);
    Vector3& operator/=(float scalar);

    bool operator==(const Vector3& other) const;
};
