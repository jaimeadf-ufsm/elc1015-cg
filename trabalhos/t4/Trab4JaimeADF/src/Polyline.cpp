#include "Polyline.h"

void PolyLine::InsertPoint(const Vector2& point)
{
    m_Points.push_back(point);
}

void PolyLine::RemovePoint(std::size_t index)
{
    if (index < m_Points.size())
    {
        m_Points.erase(m_Points.begin() + index);
    }
}

Vector2& PolyLine::GetPoint(std::size_t index)
{
    return m_Points.at(index);
}

const Vector2& PolyLine::GetPoint(std::size_t index) const
{
    return m_Points.at(index);
}

std::size_t PolyLine::GetSize() const
{
    return m_Points.size();
}

void PolyLine::Clear()
{
    m_Points.clear();
}
