#pragma once

#include "Path.h"
#include "Color.h"
#include "TesselatedGraphic.h"

class VectorFeature
{
public:
    std::vector<Path> Paths;

    VectorFeature();

    void SetFill(ColorRGB color);
    void SetStroke(ColorRGB color);

    void ClearFill();
    void ClearStroke();

    void SetStrokeWidth(float width);

    bool HasFill() const;
    bool HasStroke() const;

    ColorRGB GetStrokeColor() const;
    ColorRGB GetFillColor() const;

    float GetStrokeWidth() const;

    VectorFeature& WithPaths(const std::string& d);
    VectorFeature& WithPaths(const std::vector<Path>& paths);
    VectorFeature& WithFill(const ColorRGB& color);
    VectorFeature& WithStroke(const ColorRGB& color, float width);

private:
    bool m_FillEnable = false;
    bool m_StrokeEnable = false;

    ColorRGB m_FillColor;
    ColorRGB m_StrokeColor;

    float m_StrokeWidth = 0.0f;
};

class VectorGraphic
{
public:
    VectorGraphic();
    VectorGraphic(const std::vector<VectorFeature>& features);

    std::vector<VectorFeature> Features;

    TesselatedGraphic Materialize();
};