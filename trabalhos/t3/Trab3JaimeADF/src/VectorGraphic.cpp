#include "VectorGraphic.h"
#include "Tesselator.h"
#include "PathParser.h"

VectorFeature::VectorFeature()
{
}

void VectorFeature::SetFill(ColorRGB color)
{
    m_FillEnable = true;
    m_FillColor = color;
}

void VectorFeature::SetStroke(ColorRGB color)
{
    m_StrokeEnable = true;
    m_StrokeColor = color;
}

void VectorFeature::ClearFill()
{
    m_FillEnable = false;
    m_FillColor = ColorRGB();
}

void VectorFeature::ClearStroke()
{
    m_StrokeEnable = false;
    m_StrokeColor = ColorRGB();
}

void VectorFeature::SetStrokeWidth(float width)
{
    m_StrokeWidth = width;
}

bool VectorFeature::HasFill() const
{
    return m_FillEnable;
}

bool VectorFeature::HasStroke() const
{
    return m_StrokeEnable;
}

ColorRGB VectorFeature::GetFillColor() const
{
    return m_FillColor;
}

ColorRGB VectorFeature::GetStrokeColor() const
{
    return m_StrokeColor;
}

float VectorFeature::GetStrokeWidth() const
{
    return m_StrokeWidth;
}

VectorFeature& VectorFeature::WithPaths(const std::string& d)
{
    return WithPaths(PathParser::ParseCommands(d));
}

VectorFeature& VectorFeature::WithPaths(const std::vector<Path>& paths)
{
    Paths = paths;
    return *this;
}

VectorFeature& VectorFeature::WithFill(const ColorRGB& color)
{
    SetFill(color);
    return *this;
}

VectorFeature& VectorFeature::WithStroke(const ColorRGB& color, float width)
{
    SetStroke(color);
    SetStrokeWidth(width);
    return *this;
}

VectorGraphic::VectorGraphic()
{

}

VectorGraphic::VectorGraphic(const std::vector<VectorFeature>& features) :
    Features(features)
{

}

TesselatedGraphic VectorGraphic::Materialize()
{
    TesselatedGraphic tesselated;

    for (const VectorFeature& feature : Features)
    {
        for (const Path& path : feature.Paths)
        {
            Mesh mesh;

            if (feature.HasFill())
            {
                Tesselator::Fill(mesh, path);
                tesselated.Features.emplace_back(mesh, feature.GetFillColor());
            }

            if (feature.HasStroke())
            {
                Tesselator::Stroke(mesh, path, feature.GetStrokeWidth());
                tesselated.Features.emplace_back(mesh, feature.GetStrokeColor());
            }
        }
    }

    return tesselated;
}