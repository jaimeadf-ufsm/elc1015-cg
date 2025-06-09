#include "Keyboard.h"

#include <cctype>

std::array<bool, 256> Keyboard::s_KeyStates = {};

bool Keyboard::IsKeyDown(int key)
{
    return GetKeyState(key);
}

bool Keyboard::IsKeyUp(int key)
{
    return !GetKeyState(key);
}

void Keyboard::PressKey(int key)
{
    SetKeyState(key, true);
}

void Keyboard::ReleaseKey(int key)
{
    SetKeyState(key, false);
}

bool Keyboard::GetKeyState(int key)
{
    if (key < 0 || key >= static_cast<int>(s_KeyStates.size()))
    {
        return false;
    }

    return s_KeyStates[std::tolower(key)];
}

void Keyboard::SetKeyState(int key, bool state)
{
    if (key < 0 || key >= static_cast<int>(s_KeyStates.size()))
    {
        return;
    }

    s_KeyStates[std::tolower(key)] = state;
}
