#pragma once

#include "Axis.h"

namespace yap
{
    class BoxPadding
    {
    public:
        float Top;
        float Right;
        float Bottom;
        float Left;

        BoxPadding()
            : Top(0), Right(0), Bottom(0), Left(0) {}

        BoxPadding(float all) :
            Top(all), Right(all), Bottom(all), Left(all) {}

        BoxPadding(float horizontal, float vertical) : 
            Top(vertical), Right(horizontal), Bottom(vertical), Left(horizontal) {}
        
        BoxPadding(float top, float right, float bottom, float left) :
            Top(top), Right(right), Bottom(bottom), Left(left) {}
    
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