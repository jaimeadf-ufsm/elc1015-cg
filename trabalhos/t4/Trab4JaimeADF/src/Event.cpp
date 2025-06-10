#include "Event.h"

Event Event::CreateWindowResizeEvent(std::size_t width, std::size_t height)
{
    Event event {};
    event.Type = EventType::WindowResize;
    event.Window.Width = width;
    event.Window.Height = height;

    return event;
}

Event Event::CreateMouseMoveEvent(const Vector2& position)
{
    Event event {};
    event.Type = EventType::MouseMove;
    event.Mouse.Button = -1;
    event.Mouse.Position = position;

    return event;
}

Event Event::CreateMouseScrollEvent(const Vector2& direction, const Vector2& position)
{
    Event event {};
    event.Type = EventType::MouseScroll;
    event.Mouse.Button = -1;
    event.Mouse.Position = position;
    event.Mouse.Scroll = direction;

    return event;
}

Event Event::CreateMouseButtonPressEvent(int button, const Vector2& position)
{
    Event event {};
    event.Type = EventType::MouseButtonPress;
    event.Mouse.Button = button;
    event.Mouse.Position = position;

    return event;
}

Event Event::CreateMouseButtonReleaseEvent(int button, const Vector2& position)
{
    Event event {};
    event.Type = EventType::MouseButtonRelease;
    event.Mouse.Button = button;
    event.Mouse.Position = position;

    return event;
}

Event Event::CreateKeyPressEvent(int key)
{
    Event event {};
    event.Type = EventType::KeyPress;
    event.Keyboard.Key = key;

    return event;
}

Event Event::CreateKeyReleaseEvent(int key)
{
    Event event {};
    event.Type = EventType::KeyRelease;
    event.Keyboard.Key = key;

    return event;
}
