#pragma once

#include "Color.h"
#include "Mesh.h"
#include "Transform.h"
#include "DrawingContext.h"

class TesselatedFeature
{
public:
    TesselatedFeature();
    TesselatedFeature(const Mesh& mesh, const ColorRGB& color);

    void Translate(const Vector2& offset);

    void SetMesh(Mesh&& mesh);
    void SetColor(ColorRGB color);

    const Mesh& GetMesh() const;
    const ColorRGB& GetColor() const;

private:
    Mesh m_Mesh;
    ColorRGB m_Color;
};

class TesselatedGraphic
{
public:
    std::vector<TesselatedFeature> Features;

    void Translate(const Vector2& offset);

    void Render(DrawingContext& context);
    void Draw(DrawingContext& context, Transform transform);
};