/**
 * @file Mesh.h
 * @brief 3D mesh class for geometry storage and generation
 */

#pragma once

#include <vector>

#include "Vector4.h"
#include "Color.h"
#include "Polyline.h"
#include "Shader.h"

/**
 * @class Mesh
 * @brief Container for 3D mesh data with vertex and index arrays
 */
class Mesh
{
public:
    std::vector<Vertex> Vertices;
    std::vector<std::size_t> Indices;

    void Clear();

    static void GenerateArrow(Mesh &mesh, int xResolution);
    static void GenerateSphere(Mesh &mesh, int xResolution, int yResolution);
    static void GenerateRevolution(Mesh &mesh, const PolyLine2D& polyline, int xResolution);
    static void GenerateSweep(Mesh &mesh, const PolyLine2D& shape, const PolyLine3D& path);
};