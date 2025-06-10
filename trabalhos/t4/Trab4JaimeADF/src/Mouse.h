/**
 * @file Mouse.h
 * @brief Mouse input management and position tracking
 */

#pragma once

#include "Vector2.h"

#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_MIDDLE 1
#define MOUSE_BUTTON_RIGHT 2

/**
 * @class Mouse
 * @brief Static class for managing mouse position and input
 */
class Mouse
{
public:
    static Vector2 GetPosition();
    static void SetPosition(const Vector2& position);

private:
    static Vector2 s_Position;
};