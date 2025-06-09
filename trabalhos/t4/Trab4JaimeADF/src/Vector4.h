#pragma once

#include "Vector2.h"
#include "Vector3.h"

struct ColorRGBA;

struct Vector4
{
    float X;
    float Y;
    float Z;
    float W;

    Vector4();
    Vector4(const Vector2& xy, float z, float w);
    Vector4(const Vector3& xyz, float w = 1.0f);
    Vector4(const ColorRGBA& color);
    Vector4(float x, float y, float z, float w = 1.0f);

    Vector4 Normalize() const;

    float Dot(const Vector4& other) const;

    float Magnitude() const;
    float MagnitudeSquared() const;

    Vector3 ToCartesian() const;

    Vector4 operator+(const Vector4& other) const;
    Vector4 operator-(const Vector4& other) const;
    Vector4 operator*(const Vector4& other) const;
    Vector4 operator/(const Vector4& other) const;

    Vector4 operator*(float scalar) const;
    Vector4 operator/(float scalar) const;

    Vector4& operator+=(const Vector4& other);
    Vector4& operator-=(const Vector4& other);
    Vector4& operator*=(const Vector4& other);
    Vector4& operator/=(const Vector4& other);

    Vector4& operator*=(float scalar);
    Vector4& operator/=(float scalar);
};
