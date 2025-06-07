#include "Panel.h"

Vector2 Panel::GetSize() const
{
    return m_Size;
}

Vector2 Panel::GetPosition() const
{
    return m_Position;
}

void Panel::SetSize(const Vector2& size)
{
    m_Size = size;
}

void Panel::SetPosition(const Vector2& position)
{
    m_Position = position;
}

bool Panel::IsPointInside(const Vector2& point) const
{
    return IsPointInRectangle(point, m_Position, m_Size);
}