#include "Mesh.h"

void Mesh::Translate(const Vector2& offset)
{
    for (Vector2& vertex : Vertices)
    {
        vertex += offset;
    }
}

void Mesh::Clear()
{
    Vertices.clear();
    Triangles.clear();
}