#include "Track.h"
#include "Interpolation.h"
#include "Tesselator.h"

Track::Track() : Track(200.0f, {})
{
    Vector2 anchor = Vector2(1280 / 2 - 250, 720 / 2);

    m_Points = {
        anchor + Vector2(0.0f, 0.0f),
        anchor + Vector2(0.0f, -250.0f),
        anchor + Vector2(500.0f, -250.0f),
        anchor + Vector2(500.0f, 0.0f),
        anchor + Vector2(500.0f, 250.0f),
        anchor + Vector2(0.0f, 250.0f)
    };
}

Track:: Track(float width, const std::vector<Vector2>& points) :
    m_Dirty(true),
    m_Width(width),
    m_Points(points)
{

}

void Track::SetWidth(float width)
{
    m_Dirty = true;
    m_Width = width;
}

void Track::SetPoints(const std::vector<Vector2>& points)
{
    m_Dirty = true;
    m_Points = points;
}

float Track::GetWidth() const
{
    return m_Width;
}

const std::vector<Vector2>& Track::GetPoints() const
{
    return m_Points;
}

const Path& Track::ComputeCurve()
{
    RefreshGeometry();
    return m_CurvePath;
}

const TesselatedGraphic& Track::ComputeGraphic()
{
    RefreshGeometry();
    return m_Graphic;
}

const Mesh& Track::ComputeCourseMesh()
{
    RefreshGeometry();
    return m_CourseMesh;
}

const Mesh& Track::ComputeInnerBoundaryMesh()
{
    RefreshGeometry();
    return m_InnerBoundaryMesh;
}

const Mesh& Track::ComputeOuterBoundaryMesh()
{
    RefreshGeometry();
    return m_OuterBoundaryMesh;
}

void Track::RefreshGeometry()
{
    if (!m_Dirty)
    {
        return;
    }

    float halfWidth = m_Width / 2.0f;
    
    Path outerBoundaryPath;
    Path innerBoundaryPath;

    m_Dirty = false;

    m_CurvePath.Clear();
    m_CurvePath.MoveTo(m_Points[0]);

    for (size_t segmentIndex = 0; segmentIndex < m_Points.size() / 3; segmentIndex++)
    {
        Vector2 c0 = m_Points[(segmentIndex * 3 + 1) % m_Points.size()];
        Vector2 c1 = m_Points[(segmentIndex * 3 + 2) % m_Points.size()];
        Vector2 p1 = m_Points[(segmentIndex * 3 + 3) % m_Points.size()];

        m_CurvePath.CubicBezierTo(c0, c1, p1);
    }

    m_CurvePath.Close();

    PathTransformer::Offset(outerBoundaryPath, m_CurvePath, halfWidth + 12.0f);
    PathTransformer::Offset(innerBoundaryPath, m_CurvePath, -halfWidth - 12.0f);

    Tesselator::Stroke(m_CourseMesh, m_CurvePath, m_Width);
    Tesselator::Stroke(m_OuterBoundaryMesh, outerBoundaryPath, 24.0f);
    Tesselator::Stroke(m_InnerBoundaryMesh, innerBoundaryPath, 24.0f);

    VectorGraphic vectorGraphic;

    vectorGraphic.Features.emplace_back(
        VectorFeature()
            .WithPaths({ m_CurvePath })
            .WithStroke(0x33AF67, m_Width + 24.0f)
    );

    vectorGraphic.Features.emplace_back(
        VectorFeature()
            .WithPaths({ m_CurvePath })
            .WithStroke(0x89A4A6, m_Width + 16.0f)
    );

    vectorGraphic.Features.emplace_back(
        VectorFeature()
            .WithPaths({ m_CurvePath })
            .WithStroke(0x7D9698, m_Width + 8.0f)
    );

    vectorGraphic.Features.emplace_back(
        VectorFeature()
            .WithPaths({ m_CurvePath })
            .WithStroke(0x89A4A6, m_Width)
    );

    const std::vector<Vector2>& curvePoints = m_CurvePath.GetPoints();

    std::vector<Path> dashes;

    float dashAccumulatedLength = 0.0f;
    float dashLength = 20.0f;
    bool dashToggle = false;

    Vector2 previousPoint = curvePoints.front();

    for (size_t i = 1; i < curvePoints.size(); i++)
    {
        Vector2 currentPoint = curvePoints[i];
        float length = (currentPoint - previousPoint).Magnitude();

        dashAccumulatedLength += length;

        if (dashAccumulatedLength > dashLength)
        {
            Vector2 interpolatedPoint = Lerp(
                previousPoint,
                currentPoint,
                (dashLength - (dashAccumulatedLength - length)) / length
            );

            if (dashToggle)
            {
                dashes.back().LineTo(interpolatedPoint);
            }

            dashAccumulatedLength = 0.0f;
            dashToggle = !dashToggle;

            if (dashToggle)
            {
                dashes.emplace_back();
                dashes.back().MoveTo(interpolatedPoint);
            }

            previousPoint = interpolatedPoint;
            i = i - 1;

            continue;
        }

        if (dashToggle)
        {
            dashes.back().LineTo(currentPoint);
        }

        previousPoint = currentPoint;
    }

    vectorGraphic.Features.emplace_back(
        VectorFeature()
            .WithPaths(dashes)
            .WithStroke(0xFFCC00, 4.0f)
    );

    m_Graphic = vectorGraphic.Materialize();
}