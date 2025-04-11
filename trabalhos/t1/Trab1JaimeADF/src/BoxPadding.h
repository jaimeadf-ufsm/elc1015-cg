#pragma once

// Summary:
// This file defines the `BoxPadding` class, which represents padding values for a rectangular box.
// The class provides constructors for various initialization scenarios and methods to retrieve padding
// values along specific axes.

#include "Axis.h"

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
        float Top;    ///< Padding value for the top side of the box.
        float Right;  ///< Padding value for the right side of the box.
        float Bottom; ///< Padding value for the bottom side of the box.
        float Left;   ///< Padding value for the left side of the box.

        /**
         * @brief Default constructor. Initializes all padding values to 0.
         */
        BoxPadding()
            : Top(0), Right(0), Bottom(0), Left(0) {}

        /**
         * @brief Constructor to initialize all padding values to the same value.
         * @param all The padding value to be applied to all sides.
         */
        BoxPadding(float all) :
            Top(all), Right(all), Bottom(all), Left(all) {}

        /**
         * @brief Constructor to initialize horizontal and vertical padding values.
         * @param horizontal The padding value for the left and right sides.
         * @param vertical The padding value for the top and bottom sides.
         */
        BoxPadding(float horizontal, float vertical) : 
            Top(vertical), Right(horizontal), Bottom(vertical), Left(horizontal) {}
        
        /**
         * @brief Constructor to initialize padding values for each side individually.
         * @param top The padding value for the top side.
         * @param right The padding value for the right side.
         * @param bottom The padding value for the bottom side.
         * @param left The padding value for the left side.
         */
        BoxPadding(float top, float right, float bottom, float left) :
            Top(top), Right(right), Bottom(bottom), Left(left) {}
    
        /**
         * @brief Calculates the total padding along a specified axis.
         * @param axis The axis (X or Y) along which to calculate the total padding.
         * @return The total padding along the specified axis.
         */
        float GetTotalPaddingAlongAxis(Axis axis) const
        {
            return (axis == Axis::X) ? Left + Right : Top + Bottom;
        }

        /**
         * @brief Retrieves the start padding value along a specified axis.
         * @param axis The axis (X or Y) for which to retrieve the start padding.
         * @return The start padding value along the specified axis.
         */
        float GetStartPaddingAlongAxis(Axis axis) const
        {
            return (axis == Axis::X) ? Left : Top;
        }

        /**
         * @brief Retrieves the end padding value along a specified axis.
         * @param axis The axis (X or Y) for which to retrieve the end padding.
         * @return The end padding value along the specified axis.
         */
        float GetEndPaddingAlongAxis(Axis axis) const
        {
            return (axis == Axis::X) ? Right : Bottom;
        }
    };
}