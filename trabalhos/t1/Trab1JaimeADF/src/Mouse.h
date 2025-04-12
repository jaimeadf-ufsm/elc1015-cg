#pragma once

#include "Vec2.h"

namespace yap
{
    enum class MouseButton
    {
        Left = 0,
        Right = 1,
        Middle = 2
    };

    enum class MouseScrollDirection
    {
        Down = 0,
        Up = 1
    };

    class Mouse
    {
    public:
        Vec2 Position;
    };
}