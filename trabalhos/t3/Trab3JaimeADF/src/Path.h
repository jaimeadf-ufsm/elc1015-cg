#pragma once

#include <vector>
#include "Vector2.h"

class Path
{
public:
    Path();

    void Initialize(Vector2 point);

    void LineTo(Vector2 point);
    void CubicBezierTo(Vector2 point1, Vector2 point2, Vector2 point3);

    void Close();

    const std::vector<Vector2>& GetPoints() const;
    bool IsClosed() const;

private:
    bool m_Closed = false;
    std::vector<Vector2> m_Points;
};