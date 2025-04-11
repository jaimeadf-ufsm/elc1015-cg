#pragma once

/**
 * @file Axis.h
 * @brief Defines the Axis enumeration and utility functions for axis manipulation.
 */

namespace yap
{
    /**
     * @enum Axis
     * @brief Represents the two primary axes: X and Y.
     */
    enum class Axis
    {
        X, ///< Represents the X-axis.
        Y  ///< Represents the Y-axis.
    };

    /**
     * @brief Returns the complementary axis of the given axis.
     * 
     * @param axis The input axis (either Axis::X or Axis::Y).
     * @return Axis The complementary axis (Axis::Y if input is Axis::X, and vice versa).
     */
    Axis GetComplementAxis(Axis axis)
    {
        return (axis == Axis::X) ? Axis::Y : Axis::X;
    }
}