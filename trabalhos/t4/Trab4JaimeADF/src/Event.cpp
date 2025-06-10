#include "Event.h"

Event Event::CreateWindowResizeEvent(std::size_t width, std::size_t height)
{
    Event event = {
        .Type = EventType::WindowResize,
        .Window = {
            .Width = width,
            .Height = height
        }
    };

    return event;
}

Event Event::CreateMouseMoveEvent(const Vector2& position)
{
    Event event = {
        .Type = EventType::MouseMove,
        .Mouse = {
            .Button = -1,
            .Position = position
        }
    };

    return event;
}

Event Event::CreateMouseScrollEvent(const Vector2& direction, const Vector2& position)
{
    Event event = {
        .Type = EventType::MouseScroll,
        .Mouse = {
            .Button = -1,
            .Position = position,
            .Scroll = direction
        }
    };

    return event;
}

Event Event::CreateMouseButtonPressEvent(int button, const Vector2& position)
{
    Event event = {
        .Type = EventType::MouseButtonPress,
        .Mouse = {
            .Button = button,
            .Position = position
        }
    };

    return event;
}

Event Event::CreateMouseButtonReleaseEvent(int button, const Vector2& position)
{
    Event event = {
        .Type = EventType::MouseButtonRelease,
        .Mouse = {
            .Button = button,
            .Position = position
        }
    };

    return event;
}

Event Event::CreateKeyPressEvent(int key)
{
    Event event = {
        .Type = EventType::KeyPress,
        .Keyboard = {
            .Key = key
        }
    };

    return event;
}

Event Event::CreateKeyReleaseEvent(int key)
{
    Event event = {
        .Type = EventType::KeyRelease,
        .Keyboard = {
            .Key = key
        }
    };

    return event;
}
