
#include "Mouse.h"

void Mouse::SetPosition(Vector2 position)
{
    m_Position = position;
}

void Mouse::SetButtonState(uint8_t button, bool pressed)
{
    if (button >= 0 && button < m_Buttons.size())
    {
        m_Buttons[button] = pressed;
    }
}

Vector2 Mouse::GetPosition() const
{
    return m_Position;
}

bool Mouse::IsButtonPressed(uint8_t button) const
{
    if (button >= 0 && button < m_Buttons.size())
    {
        return m_Buttons[button];
    }
    
    return false;
}
