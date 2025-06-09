#pragma once

#include <vector>

#include "Vector4.h"
#include "Color.h"
#include "Polyline.h"
#include "Shader.h"

class Mesh
{
public:
    std::vector<Vertex> Vertices;
    std::vector<std::size_t> Indices;

    void Clear();

    static void GenerateArrow(Mesh &mesh, int arcSegments);
    static void GenerateSphere(Mesh &mesh, int xSegments, int ySegments);
    static void GenerateRevolution(Mesh &mesh, const PolyLine& polyline, int arcSegments);
};