#pragma once

#include <array>
#include <cstdint>

#define KEY_W 'w'
#define KEY_A 'a'
#define KEY_S 's'
#define KEY_D 'd'

class Keyboard
{
public:
    void SetKeyState(uint8_t key, bool pressed);
    bool IsKeyPressed(uint8_t key) const;

private:
    std::array<bool, 256> m_Keys;
};