#pragma once

#include <cstddef>
#include "Vector2.h"

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

struct MouseData
{
    int Button;
    Vector2 Position;
    Vector2 Scroll;
};

struct KeyboardData
{
    int Key;
};

struct WindowData
{
    std::size_t Width;
    std::size_t Height;
};

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