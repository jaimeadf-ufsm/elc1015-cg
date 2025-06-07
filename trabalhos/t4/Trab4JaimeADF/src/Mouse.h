#pragma once

#include "Vector2.h"

#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_MIDDLE 1
#define MOUSE_BUTTON_RIGHT 2

class Mouse
{
public:
    static Vector2 GetPosition();
    static void SetPosition(const Vector2& position);

private:
    static Vector2 s_Position;
};