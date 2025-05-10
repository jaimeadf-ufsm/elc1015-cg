#pragma once

#include "Vector2.h"

enum class EventType
{
    MouseMove,
    MouseButtonPress,
    MouseButtonRelease,
    KeyPress,
    KeyRelease
};

struct MouseMoveData
{
    Vector2 Position;
};

struct MouseData
{
    Vector2 Position;
    int Button;
};

struct KeyData
{
    int Key;
};

struct Event
{
public:
    EventType Type;
    union
    {
        MouseData Mouse;
        KeyData Key;
    };

    static Event CreateMouseMoveEvent(Vector2 position)
    {
        Event event;
        event.Type = EventType::MouseMove;
        event.Mouse.Position = position;
        event.Mouse.Button = 0;

        return event;
    }

    static Event CreateMouseButtonPressEvent(Vector2 position, int button)
    {
        Event event;
        event.Type = EventType::MouseButtonPress;
        event.Mouse.Position = position;
        event.Mouse.Button = button;

        return event;
    }

    static Event CreateMouseButtonReleaseEvent(Vector2 position, int button)
    {
        Event event;
        event.Type = EventType::MouseButtonRelease;
        event.Mouse.Position = position;
        event.Mouse.Button = button;

        return event;
    }

    static Event CreateKeyPressEvent(int key)
    {
        Event event;
        event.Type = EventType::KeyPress;
        event.Key.Key = key;

        return event;
    }

    static Event CreateKeyReleaseEvent(int key)
    {
        Event event;
        event.Type = EventType::KeyRelease;
        event.Key.Key = key;

        return event;
    }

private:
    Event()
    {
    }
};