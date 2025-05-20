#include "TesselatedGraphic.h"

TesselatedFeature::TesselatedFeature()
{
}

TesselatedFeature::TesselatedFeature(const Mesh& mesh, const ColorRGB& color) :
    m_Mesh(mesh),
    m_Color(color)
{
}

void TesselatedFeature::Translate(const Vector2& offset)
{
    m_Mesh.Translate(offset);
}

void TesselatedFeature::SetMesh(Mesh&& mesh)
{
    m_Mesh = mesh;
}

void TesselatedFeature::SetColor(ColorRGB color)
{
    m_Color = color;
}

const Mesh& TesselatedFeature::GetMesh() const
{
    return m_Mesh;
}

const ColorRGB& TesselatedFeature::GetColor() const
{
    return m_Color;
}

void TesselatedGraphic::Translate(const Vector2& offset)
{
    for (TesselatedFeature& feature : Features)
    {
        feature.Translate(offset);
    }
}

void TesselatedGraphic::Draw(DrawingContext& context) const
{
    Draw(context, Transform());
}

void TesselatedGraphic::Draw(DrawingContext& context, Transform transform) const
{
    for (const TesselatedFeature& feature : Features)
    {
        const Mesh& mesh = feature.GetMesh();
        const ColorRGB& color = feature.GetColor();

        context.Color(color);

        for (size_t i = 0; i + 3 <= mesh.Triangles.size(); i += 3)
        {
            size_t indexA = mesh.Triangles[i];
            size_t indexB = mesh.Triangles[i + 1];
            size_t indexC = mesh.Triangles[i + 2];

            Vector2 vertexA;
            Vector2 vertexB;
            Vector2 vertexC;

            transform.Apply(mesh.Vertices[indexA], vertexA);
            transform.Apply(mesh.Vertices[indexB], vertexB);
            transform.Apply(mesh.Vertices[indexC], vertexC);

            context.BeginPolygon();
            context.Vertex(vertexA);
            context.Vertex(vertexB);
            context.Vertex(vertexC);
            context.FillPolygon();
        }
    }
}
