#pragma once

#include "Axis.h"

namespace yap
{
    class BoxPadding
    {
    public:
        float Left;
        float Right;
        float Top;
        float Bottom;

        BoxPadding()
            : Left(0), Right(0), Top(0), Bottom(0) {}

        BoxPadding(float all)
            : Left(all), Right(all), Top(all), Bottom(all) {}

        BoxPadding(float horizontal, float vertical) : 
            Left(horizontal), Right(horizontal), Top(vertical), Bottom(vertical) {}

        BoxPadding(float left, float right, float top, float bottom) :
            Left(left), Right(right), Top(top), Bottom(bottom) {}
    
        float GetTotalPaddingAlongAxis(Axis axis) const
        {
            return (axis == Axis::X) ? Left + Right : Top + Bottom;
        }

        float GetStartPaddingAlongAxis(Axis axis) const
        {
            return (axis == Axis::X) ? Left : Top;
        }

        float GetEndPaddingAlongAxis(Axis axis) const
        {
            return (axis == Axis::X) ? Right : Bottom;
        }
    };
}