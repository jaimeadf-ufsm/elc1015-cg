#include <string>

#include "Keyboard.h"

void Keyboard::SetKeyState(uint8_t key, bool pressed)
{
    if (key >= 0 && key < (int)m_Keys.size())
    {
        if (key >= 'A' && key <= 'Z')
        {
            key = std::tolower(key);
        }

        m_Keys[key] = pressed;
    }
}

bool Keyboard::IsKeyPressed(uint8_t key) const
{
    if (key >= 0 && key < m_Keys.size())
    {
        return m_Keys[key];
    }
    
    return false;
}
