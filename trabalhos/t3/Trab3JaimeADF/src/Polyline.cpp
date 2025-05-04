#include "Polyline.h"

PolyLine::PolyLine() : m_Closed(false) {}

void PolyLine::AddPoint(const Vector2& point)
{
    m_Points.push_back(point);
}

void PolyLine::Close()
{
    m_Closed = true;

    if (!m_Points.empty() && m_Points.front() == m_Points.back())
    {
        m_Points.pop_back();
    }
}

void PolyLine::Clear()
{
    m_Points.clear();
    m_Closed = false;
}

bool PolyLine::IsClosed() const
{
    return m_Closed;
}

const std::vector<Vector2>& PolyLine::GetPoints() const
{
    return m_Points;
}

