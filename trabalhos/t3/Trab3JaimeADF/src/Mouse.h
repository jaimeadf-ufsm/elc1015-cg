#pragma once

#include <array>
#include <cstdint>

#include "Vector2.h"

#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_MIDDLE 1
#define MOUSE_BUTTON_RIGHT 2

class Mouse
{
public:
    void SetPosition(Vector2 position);
    void SetButtonState(uint8_t button, bool pressed);

    Vector2 GetPosition() const;
    bool IsButtonPressed(uint8_t button) const;

private:
    Vector2 m_Position;
    std::array<bool, 3> m_Buttons;
};