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