#include "Shader.h"

void Shader::ProcessGeometry(const Vertex& v0, const Vertex& v1, const Vertex& v2, std::vector<VertexInput>& triangles) const
{
    triangles.push_back({ v0.Position, v0.UV, v0.Normal });
    triangles.push_back({ v1.Position, v1.UV, v1.Normal });
    triangles.push_back({ v2.Position, v2.UV, v2.Normal });
}