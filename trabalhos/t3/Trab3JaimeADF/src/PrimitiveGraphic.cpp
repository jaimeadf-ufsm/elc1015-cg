#include <cmath>

#include "PrimitiveGraphic.h"
#include "Constants.h"

void PrimitiveGraphic::FillCircle(DrawingContext& context, Vector2 center, float radius)
{
    int steps = (2.0f * PI * radius) / 2;
    float increment = 2 * PI / steps;

    context.BeginPolygon();

    for (int step = 0; step < steps; step++)
    {
        float angle = step * increment;
        Vector2 direction = Vector2(std::cos(angle), std::sin(angle));

        context.Vertex(center + direction * radius);
    }

    context.FillPolygon();
}

void PrimitiveGraphic::FillMesh(DrawingContext& context, const Mesh& mesh)
{
    for (size_t i = 0; i + 3 <= mesh.Triangles.size(); i += 3)
    {
        size_t indexA = mesh.Triangles[i];
        size_t indexB = mesh.Triangles[i + 1];
        size_t indexC = mesh.Triangles[i + 2];

        Vector2 vertexA = mesh.Vertices[indexA];
        Vector2 vertexB = mesh.Vertices[indexB];
        Vector2 vertexC = mesh.Vertices[indexC];

        context.BeginPolygon();
        context.Vertex(vertexA);
        context.Vertex(vertexB);
        context.Vertex(vertexC);
        context.FillPolygon();
    }
}