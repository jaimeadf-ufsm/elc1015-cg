#pragma once

#include "Axis.h"

/**
 * @file BoxPadding.h
 * @brief Defines a class to represent the padding values for a box element.
 */

namespace yap
{
    /**
     * @class BoxPadding
     * @brief Represents padding values for a rectangular box.
     *
     * The `BoxPadding` class provides a way to define and manipulate padding values for a box.
     * Padding values can be specified for each side (top, right, bottom, left) individually or uniformly.
     * The class also provides utility methods to retrieve total, start, and end padding along a specific axis.
     */
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