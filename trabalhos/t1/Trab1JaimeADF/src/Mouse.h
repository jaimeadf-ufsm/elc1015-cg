#pragma once

#include "Vec2.h"

/**
 * @file Mouse.h
 * @brief Defines the Mouse class and related enumerations for mouse input handling.
 * 
 * This file contains the Mouse class, which represents the state of a mouse, 
 * including its position. It also defines enumerations for mouse buttons and 
 * scroll directions to facilitate input handling.
 */

namespace yap
{
    /**
     * @enum MouseButton
     * @brief Represents the different buttons on a mouse.
     */
    enum class MouseButton
    {
        Left = 0,
        Right = 1,
        Middle = 2
    };

    /**
     * @enum MouseScrollDirection
     * @brief Represents the direction of mouse scroll input.
     */
    enum class MouseScrollDirection
    {
        Down = 0,
        Up = 1
    };

    /**
     * @class Mouse
     * @brief Represents the state of a mouse, including its position.
     */
    class Mouse
    {
    public:
        Vec2 Position;
    };
}