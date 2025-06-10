/**
 * @file Shader.h
 * @brief Base shader class and vertex/fragment data structures
 */

#pragma once

#include <vector>
#include <algorithm>

#include "Matrix4x4.h"
#include "Color.h"

/**
 * @struct Vertex
 * @brief Vertex data structure with position, UV coordinates, and normal
 */
struct Vertex
{
    Vector3 Position;
    Vector2 UV;
    Vector3 Normal;
};

/**
 * @struct VertexInput
 * @brief Input data structure for vertex shader processing
 */
struct VertexInput
{
    Vector3 Position;
    Vector2 UV;
    Vector3 Normal;
};

/**
 * @struct VertexOutput
 * @brief Output data structure from vertex shader processing
 */
struct VertexOutput
{
    Vector4 ClipPosition;
    Vector3 WorldPosition;
    Vector2 UV;
    Vector3 Normal;
};

/**
 * @struct FragmentInput
 * @brief Input data structure for fragment shader processing
 */
struct FragmentInput
{
    Vector3 Barycentric;
    Vector2 ScreenCoordinate;

    Vector3 WorldPosition;
    Vector2 UV;
    Vector3 Normal;
};

/**
 * @struct FragmentOutput
 * @brief Output data structure from fragment shader processing
 */
struct FragmentOutput
{
    Vector4 Color;
};

/**
 * @class Shader
 * @brief Abstract base class for shader programs with vertex and fragment processing
 */
class Shader
{
public:
    virtual void ProcessGeometry(const Vertex& v0, const Vertex& v1, const Vertex& v2, std::vector<VertexInput>& triangles) const;

    virtual VertexOutput ProcessVertex(const VertexInput& input) const = 0;
    virtual FragmentOutput ProcessFragment(const FragmentInput& input) const = 0;
};