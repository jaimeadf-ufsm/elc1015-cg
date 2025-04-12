#pragma once

#include "Axis.h"

/**
 * @file Vec2.h
 * @brief Defines a 2D vector class with various mathematical operations and utilities.
 * 
 * This file contains the `Vec2` struct, which represents a 2D vector with `X` and `Y` components.
 * It provides a wide range of methods for vector manipulation, including rotation, normalization,
 * clamping, and arithmetic operations.
 */

namespace yap
{
    /**
     * @struct Vec2
     * @brief Represents a 2D vector with `X` and `Y` components.
     * 
     * The `Vec2` struct provides methods for common vector operations such as rotation,
     * normalization, dot product, and more. It also supports operator overloading for
     * arithmetic operations.
     */
    struct Vec2
    {
        float X, Y;

        Vec2() : X(0), Y(0) {}
        Vec2(float value) : X(value), Y(value) {}
        Vec2(float x, float y) : X(x), Y(y) {}
        Vec2(const Vec2& other) : X(other.X), Y(other.Y) {}

        void Rotate(float angle)
        {
            float cosAngle = cos(angle);
            float sinAngle = sin(angle);

            float newX = X * cosAngle - Y * sinAngle;
            float newY = X * sinAngle + Y * cosAngle;

            X = newX;
            Y = newY;
        }

        void Rotate(float angle, const Vec2& pivot)
        {
            float cosAngle = cos(angle);
            float sinAngle = sin(angle);

            float newX = (X - pivot.X) * cosAngle - (Y - pivot.Y) * sinAngle + pivot.X;
            float newY = (X - pivot.X) * sinAngle + (Y - pivot.Y) * cosAngle + pivot.Y;

            X = newX;
            Y = newY;
        }

        void Normalize()
        {
            float length = Length();

            if (length == 0)
            {
                X = 0;
                Y = 0;
            }

            X /= length;
            Y /= length;
        }

        float Length() const
        {
            return sqrt(X * X + Y * Y);
        }

        float LengthSquared() const
        {
            return X * X + Y * Y;
        }

        float Angle() const
        {
            return atan2(Y, X);
        }

        float Dot(const Vec2& other) const
        {
            return X * other.X + Y * other.Y;
        }

        void Clamp(float min, float max)
        {
            X = std::max(min, std::min(X, max));
            Y = std::max(min, std::min(Y, max));
        }

        void Floor()
        {
            X = floor(X);
            Y = floor(Y);
        }

        Vec2& operator+=(const Vec2& other)
        {
            X += other.X;
            Y += other.Y;
            return *this;
        }

        Vec2& operator+=(float scalar)
        {
            X += scalar;
            Y += scalar;
            return *this;
        }

        Vec2& operator-=(const Vec2& other)
        {
            X -= other.X;
            Y -= other.Y;
            return *this;
        }

        Vec2& operator-=(float scalar)
        {
            X -= scalar;
            Y -= scalar;
            return *this;
        }

        Vec2& operator*=(const Vec2& other)
        {
            X *= other.X;
            Y *= other.Y;
            return *this;
        }

        Vec2& operator*=(float scalar)
        {
            X *= scalar;
            Y *= scalar;
            return *this;
        }

        Vec2& operator/=(const Vec2& other)
        {
            X /= other.X;
            Y /= other.Y;
            return *this;
        }

        Vec2& operator/=(float scalar)
        {
            X /= scalar;
            Y /= scalar;
            return *this;
        }

        Vec2 operator-() const
        {
            return Vec2(-X, -Y);
        }

        Vec2 operator+(const Vec2& other) const
        {
            return Vec2(X + other.X, Y + other.Y);
        }

        Vec2 operator-(const Vec2& other) const
        {
            return Vec2(X - other.X, Y - other.Y);
        }

        Vec2 operator*(const Vec2& other) const
        {
            return Vec2(X * other.X, Y * other.Y);
        }

        Vec2 operator*(float scalar) const
        {
            return Vec2(X * scalar, Y * scalar);
        }

        Vec2 operator/(const Vec2& other) const
        {
            return Vec2(X / other.X, Y / other.Y);
        }

        Vec2 operator/(float scalar) const
        {
            return Vec2(X / scalar, Y / scalar);
        }

        void SetValueAlongAxis(Axis axis, float value)
        {
            if (axis == Axis::X)
            {
                X = value;
            }
            else
            {
                Y = value;
            }
        }

        float GetValueAlongAxis(Axis axis)
        {
            return (axis == Axis::X ? X : Y);
        }

        static Vec2 Normalize(const Vec2& v)
        {
            Vec2 result = v;
            result.Normalize();

            return result;
        }

        static Vec2 Rotate(const Vec2& v, float angle)
        {
            Vec2 result = v;
            result.Rotate(angle);

            return result;
        }

        static Vec2 Rotate(const Vec2& v, float angle, const Vec2& pivot)
        {
            Vec2 result = v;
            result.Rotate(angle, pivot);

            return result;
        }
    
        static Vec2 Floor(const Vec2& v)
        {
            return Vec2(floor(v.X), floor(v.Y));
        }
    };
}