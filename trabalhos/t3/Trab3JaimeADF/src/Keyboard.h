#pragma once

#include <array>
#include <cstdint>

class Keyboard
{
public:
    void SetKeyState(uint8_t key, bool pressed);
    bool IsKeyPressed(uint8_t key) const;

private:
    std::array<bool, 256> m_Keys;
};