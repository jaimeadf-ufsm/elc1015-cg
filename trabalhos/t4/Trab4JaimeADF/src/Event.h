/**
 * @file Event.h
 * @brief Event system for handling window, mouse, and keyboard events
 */

#pragma once

#include <cstddef>
#include "Vector2.h"

/**
 * @enum EventType
 * @brief Types of events that can be generated
 */
enum class EventType
{
    WindowResize,
    KeyPress,
    KeyRelease,
    MouseMove,
    MouseScroll,
    MouseButtonPress,
    MouseButtonRelease
};

/**
 * @struct MouseData
 * @brief Data structure for mouse-related events
 */
struct MouseData
{
    int Button;
    Vector2 Position;
    Vector2 Scroll;
};

/**
 * @struct KeyboardData
 * @brief Data structure for keyboard-related events
 */
struct KeyboardData
{
    int Key;
};

/**
 * @struct WindowData
 * @brief Data structure for window-related events
 */
struct WindowData
{
    std::size_t Width;
    std::size_t Height;
};

/**
 * @struct Event
 * @brief Main event structure containing type and data union
 */
struct Event
{
    EventType Type;
    union
    {
        MouseData Mouse;
        KeyboardData Keyboard;
        WindowData Window;
    };

    static Event CreateWindowResizeEvent(std::size_t width, std::size_t height);
    static Event CreateMouseMoveEvent(const Vector2& position);
    static Event CreateMouseScrollEvent(const Vector2& direction, const Vector2& position);
    static Event CreateMouseButtonPressEvent(int button, const Vector2& position);
    static Event CreateMouseButtonReleaseEvent(int button, const Vector2& position);
    static Event CreateKeyPressEvent(int key);
    static Event CreateKeyReleaseEvent(int key);
};