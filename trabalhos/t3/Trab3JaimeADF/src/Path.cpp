#include <cmath>

#include "Path.h"

Path::Path() : m_Closed(false), m_Points({ Vector2() })
{
}

void Path::MoveTo(Vector2 point)
{
    m_Points.push_back(point);
}

void Path::LineTo(Vector2 point)
{
    m_Points.push_back(point);
}

void Path::CubicBezierTo(Vector2 point1, Vector2 point2, Vector2 point3)
{
    Vector2 p0 = m_Points.back();
    Vector2 p1 = point1;
    Vector2 p2 = point2;
    Vector2 p3 = point3;

    float t = 0.0f;

    float step = 0.05f;

    while (t < 1.0f)
    {
        t += step;

        if (t > 1.0f)
        {
            t = 1.0f;
        }

        float t1 = t;
        float t2 = t1 * t1;
        float t3 = t2 * t1;

        Vector2 p = p0;
        p += t1 * (-3 * p0 + 3 * p1);
        p += t2 * (3 * p0 - 6 * p1 + 3 * p2);
        p += t3 * (p3 - 3 * p2 + 3 * p1 - p0);

        m_Points.push_back(p);
    }
}

void Path::Arc(Vector2 center, float radius, float initialAngle, float finalAngle)
{
    int steps = 100;
    float increment = (finalAngle - initialAngle) / steps;

    for (int step = 0; step <= steps; step++)
    {
        float angle = initialAngle + increment * step;
        Vector2 point = center + radius * Vector2(std::cos(angle), std::sin(angle));

        m_Points.push_back(point);
    }
}

void Path::Close()
{
    m_Closed = true;
}

void Path::Clear()
{
    m_Closed = false;
    m_Points.clear();
}

const std::vector<Vector2>& Path::GetPoints() const
{
    return m_Points;
}

bool Path::IsClosed() const
{
    return m_Closed;
}