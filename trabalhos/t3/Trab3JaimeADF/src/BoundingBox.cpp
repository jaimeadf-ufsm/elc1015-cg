#include <limits>

#include "BoundingBox.h"

BoundingBox::BoundingBox() :
    m_MinPoint(Vector2(std::numeric_limits<float>::max(), std::numeric_limits<float>::max())),
    m_MaxPoint(Vector2(std::numeric_limits<float>::min(), std::numeric_limits<float>::min()))
{

}

void BoundingBox::Encompass(const Vector2& point)
{
    m_MinPoint.X = std::min(m_MinPoint.X, point.X);
    m_MinPoint.Y = std::min(m_MinPoint.Y, point.Y);
    m_MaxPoint.X = std::max(m_MaxPoint.X, point.X);
    m_MaxPoint.Y = std::max(m_MaxPoint.Y, point.Y);
}

Vector2 BoundingBox::GetMinPoint() const
{
    return m_MinPoint;
}

Vector2 BoundingBox::GetMaxPoint() const
{
    return m_MaxPoint;
}