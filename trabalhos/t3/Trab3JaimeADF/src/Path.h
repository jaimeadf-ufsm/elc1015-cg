#pragma once

#include <vector>
#include "Vector2.h"

class Path
{
public:
    Path();

    void MoveTo(Vector2 point = Vector2());
    void LineTo(Vector2 point);
    void QuadraticBezierTo(Vector2 point1, Vector2 point2);
    void CubicBezierTo(Vector2 point1, Vector2 point2, Vector2 point3);
    void Arc(Vector2 point, float radius, float initialAngle, float finalAngle);

    void Clear();
    void Close();

    const std::vector<Vector2>& GetPoints() const;

    bool IsClosed() const;
    bool IsEmpty() const;

private:
    bool m_Closed = false;
    std::vector<Vector2> m_Points;
};