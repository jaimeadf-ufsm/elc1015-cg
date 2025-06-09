#pragma once

#include <array>

class Keyboard
{
public:
    static bool IsKeyDown(int key);
    static bool IsKeyUp(int key);

    static void PressKey(int key);
    static void ReleaseKey(int key);

    static bool GetKeyState(int key);
    static void SetKeyState(int key, bool state);

private:
    static std::array<bool, 256> s_KeyStates;
};
