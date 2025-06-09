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
    enum class ModelType
    {
        Revolution,
        SpringSweep
    };

    static const float s_PointRadius;
    static const float s_LocalGridSpacing;

    ModelType m_ModelType;

    PolyLine2D m_TemporaryPolyLine;
    PolyLine2D m_CurvePolyLine;

    PolyLine3D m_PathPolyline;

    Bezier m_Bezier;

    int m_BezierDegree;

    int m_CurveResolution;
    int m_ExtrudeResolution;

    float m_SpringRadius;
    float m_SpringHeight;
    float m_SpringFrequency;

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

    void RegenerateCurve();
    void RegenerateModel();

    float ComputeScale() const;

    Vector2 ConvertToLocalCoordinates(const Vector2& point) const;
    Vector2 ConvertToScreenCoordinates(const Vector2& point) const;
};