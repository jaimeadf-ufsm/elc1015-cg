/**
 * @file Keyboard.h
 * @brief Keyboard input management and key state tracking
 */

#pragma once

#include <array>

/**
 * @class Keyboard
 * @brief Static class for managing keyboard input states
 */
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
