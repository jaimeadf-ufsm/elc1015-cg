#pragma once

#include <vector>
#include <algorithm>

#include "Matrix4x4.h"
#include "Color.h"

struct Vertex
{
    Vector3 Position;
    Vector2 UV;
    Vector3 Normal;
};

struct VertexInput
{
    Vector3 Position;
    Vector2 UV;
    Vector3 Normal;
};

struct VertexOutput
{
    Vector4 ClipPosition;
    Vector3 WorldPosition;
    Vector2 UV;
    Vector3 Normal;
};

struct FragmentInput
{
    Vector3 Barycentric;
    Vector2 ScreenCoordinate;

    Vector3 WorldPosition;
    Vector2 UV;
    Vector3 Normal;
};

struct FragmentOutput
{
    Vector4 Color;
};

class Shader
{
public:
    virtual void ProcessGeometry(const Vertex& v0, const Vertex& v1, const Vertex& v2, std::vector<VertexInput>& triangles) const;

    virtual VertexOutput ProcessVertex(const VertexInput& input) const = 0;
    virtual FragmentOutput ProcessFragment(const FragmentInput& input) const = 0;
};