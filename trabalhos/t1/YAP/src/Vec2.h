#pragma once

#include "Axis.h"

namespace yap
{
    struct Vec2
    {
        float X, Y;

        Vec2() : X(0), Y(0) {}
        Vec2(float value) : X(value), Y(value) {}
        Vec2(float x, float y) : X(x), Y(y) {}
        Vec2(const Vec2& other) : X(other.X), Y(other.Y) {}

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

        float Length() const
        {
            return sqrt(X * X + Y * Y);
        }

        float LengthSquared() const
        {
            return X * X + Y * Y;
        }

        float Dot(const Vec2& other) const
        {
            return X * other.X + Y * other.Y;
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
    
        static Vec2 Floor(const Vec2& v)
        {
            return Vec2(floor(v.X), floor(v.Y));
        }
    };
}