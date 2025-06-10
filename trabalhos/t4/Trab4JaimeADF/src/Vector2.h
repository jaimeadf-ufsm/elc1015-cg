/**
 * @file Vector2.h
 * @brief 2D vector class with mathematical operations
 */

#pragma once

/**
 * @struct Vector2
 * @brief 2D vector structure with X and Y components and mathematical operations
 */
struct Vector2
{
    float X;
    float Y;

    Vector2();
    Vector2(float x, float y);

    Vector2 Normalize() const;

    float Dot(const Vector2& other) const;
    float Cross(const Vector2& other) const;

    float Magnitude() const;
    float MagnitudeSquared() const;

    Vector2 Floor() const;
    Vector2 Ceil() const;
    Vector2 Round() const;

    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(const Vector2& other) const;
    Vector2 operator/(const Vector2& other) const;

    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;

    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(const Vector2& other);
    Vector2& operator/=(const Vector2& other);

    Vector2& operator*=(float scalar);
    Vector2& operator/=(float scalar);

    bool operator==(const Vector2& other) const;
};