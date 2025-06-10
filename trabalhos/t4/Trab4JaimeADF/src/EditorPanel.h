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

    int m_BezierDegree;
    bool m_BezierClosed;

    int m_CurveResolution;
    int m_ExtrudeResolution;

    float m_SpringRadius;
    float m_SpringHeight;
    float m_SpringFrequency;

    int m_SelectedPointIndex;

    ModelType m_ModelType;

    PolyLine2D m_TemporaryPolyLine;
    PolyLine2D m_BezierPolyLine;

    PolyLine3D m_PathPolyLine;

    Bezier m_Bezier;

    std::vector<Vector2> m_ControlPoints;
    std::vector<Vector2> m_BezierPoints;

    void DrawGrid();
    void DrawCurve();
    void DrawBezierPolygon();
    void DrawControlPoints();
    void DrawInformation();

    int LocatePointAt(const Vector2& screenPosition) const;

    void SelectPoint(int index);
    void DeselectPoint();

    void CreatePoint(const Vector2& screenPosition);
    void DeletePoint(int index);

    void MovePoint(int index, const Vector2& screenPosition);

    void UpdatePoints();
    void RegenerateCurve();
    void RegenerateModel();

    float ComputeScale() const;

    Vector2 ConvertToLocalCoordinates(const Vector2& point) const;
    Vector2 ConvertToScreenCoordinates(const Vector2& point) const;
};