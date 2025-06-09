#pragma once

#include "Panel.h"
#include "Bezier.h"

class EditorPanel : public Panel
{
public:
    EditorPanel();

    void Process(const Event& event) override;
    void Update() override;
    void Draw() override;

private:
    static const float s_PointRadius;
    static const float s_LocalGridSpacing;

    PolyLine m_TemporaryPolyLine;
    PolyLine m_CurvePolyLine;

    Bezier m_Bezier;

    int m_BezierDegree;
    int m_BezierSegments;
    int m_BezierArcs;

    int m_SelectedPointIndex;

    std::vector<Vector2> m_BezierPoints;

    void DrawGrid();
    void DrawCurve();
    void DrawControlPolygon();
    void DrawControlPoints();
    void DrawInformation();

    int LocatePointAt(const Vector2& screenPosition) const;

    void SelectPoint(int index);
    void DeselectPoint();

    void CreatePoint(const Vector2& screenPosition);
    void DeletePoint(int index);

    void MovePoint(int index, const Vector2& screenPosition);

    void IncreaseDegree();
    void DecreaseDegree();

    void IncreaseSamples();
    void DecreaseSamples();

    void IncreaseArcs();
    void DecreaseArcs();

    void RegenerateCurve();

    float ComputeScale() const;

    Vector2 ConvertToLocalCoordinates(const Vector2& point) const;
    Vector2 ConvertToScreenCoordinates(const Vector2& point) const;
};