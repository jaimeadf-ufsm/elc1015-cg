#pragma once

#include <vector>

#include "Vector2.h"

class Transform
{
public:
    Transform();

    void Apply(const Vector2& source, Vector2& destination) const;
    void Apply(const std::vector<Vector2>& source, std::vector<Vector2>& destination) const;

    void SetPosition(Vector2 position);
    void SetScale(Vector2 scale);
    void SetRotation(float angle);

    Vector2 GetPosition() const;
    Vector2 GetScale() const;
    float GetRotation() const;

    Vector2 GetHeading() const;

private:
    Vector2 m_Position;
    Vector2 m_Scale;
    float m_Angle;

    float m_Sin;
    float m_Cos;
};