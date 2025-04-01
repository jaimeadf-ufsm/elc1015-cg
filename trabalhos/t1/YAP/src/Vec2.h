#pragma once

#include "Axis.h"

namespace yap
{
    struct Vec2
    {
        float X, Y;

        Vec2() : X(0), Y(0) {}
        Vec2(float x, float y) : X(x), Y(y) {}
        Vec2(const Vec2& other) : X(other.X), Y(other.Y) {}

        Vec2 operator+(const Vec2& other)
        {
            return Vec2(X + other.X, Y + other.Y);
        }

        Vec2 operator-(const Vec2& other)
        {
            return Vec2(X - other.X, Y - other.Y);
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
    };
}