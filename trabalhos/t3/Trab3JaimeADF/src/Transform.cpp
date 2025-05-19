#include "Transform.h"

#include <cmath>

Transform::Transform() :
    m_Position(),
    m_Scale(Vector2(1.0f, 1.0f)),
    m_Angle(0.0f),
    m_Sin(0.0f),
    m_Cos(1.0f)
{
}

void Transform::Apply(const Vector2& source, Vector2& destination) const
{
    destination.X = (m_Cos * source.X - m_Sin * source.Y) * m_Scale.X + m_Position.X;
    destination.Y = (m_Sin * source.X + m_Cos * source.Y) * m_Scale.Y + m_Position.Y;
}

void Transform::Apply(const std::vector<Vector2>& source, std::vector<Vector2>& destination) const
{
    destination.resize(source.size());

    for (size_t i = 0; i < source.size(); i++)
    {
        Apply(source[i], destination[i]);
    }
}

void Transform::SetPosition(Vector2 position)
{
    m_Position = position;
}

void Transform::SetScale(Vector2 scale)
{
    m_Scale = scale;
}

void Transform::SetRotation(float angle)
{
    m_Angle = angle;
    m_Sin = std::sin(angle);
    m_Cos = std::cos(angle);
}

Vector2 Transform::GetPosition() const
{
    return m_Position;
}

Vector2 Transform::GetScale() const
{
    return m_Scale;
}

float Transform::GetRotation() const
{
    return m_Angle;
}

Vector2 Transform::GetHeading() const
{
    return Vector2(m_Cos, m_Sin);
}