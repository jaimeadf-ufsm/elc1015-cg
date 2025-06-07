#include "Mouse.h"

Vector2 Mouse::s_Position(0.0f, 0.0f);

Vector2 Mouse::GetPosition()
{
    return s_Position;
}

void Mouse::SetPosition(const Vector2& position)
{
    s_Position = position;
}