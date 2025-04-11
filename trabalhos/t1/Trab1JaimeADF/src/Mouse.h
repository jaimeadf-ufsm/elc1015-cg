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
        Left = 0,   /**< The left mouse button. */
        Right = 1,  /**< The right mouse button. */
        Middle = 2  /**< The middle mouse button (usually the scroll wheel button). */
    };

    /**
     * @enum MouseScrollDirection
     * @brief Represents the direction of mouse scroll input.
     */
    enum class MouseScrollDirection
    {
        Down = 0,   /**< Scrolling down. */
        Up = 1      /**< Scrolling up. */
    };

    /**
     * @class Mouse
     * @brief Represents the state of a mouse, including its position.
     */
    class Mouse
    {
    public:
        /**
         * @brief The current position of the mouse cursor.
         * 
         * This is represented as a 2D vector (Vec2) with x and y coordinates.
         */
        Vec2 Position;
    };
}