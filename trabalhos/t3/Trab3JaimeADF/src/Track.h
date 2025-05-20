#pragma once

#include "Path.h"
#include "PathTransformer.h"
#include "VectorGraphic.h"

class Track
{
public:
    Track();
    Track(float width, const std::vector<Vector2>& points);

    void SetWidth(float width);
    void SetPoints(const std::vector<Vector2>& points);

    float GetWidth() const;
    const std::vector<Vector2>& GetPoints() const;

    const Path& ComputeCurve();

    const Mesh& ComputeCourseMesh();
    const Mesh& ComputeOuterBoundaryMesh();
    const Mesh& ComputeInnerBoundaryMesh();

    const TesselatedGraphic& ComputeGraphic();

private:
    bool m_Dirty;

    float m_Width;
    std::vector<Vector2> m_Points;

    Path m_CurvePath;

    Mesh m_CourseMesh;

    Mesh m_OuterBoundaryMesh;
    Mesh m_InnerBoundaryMesh;

    TesselatedGraphic m_Graphic;

    void RefreshGeometry();
};