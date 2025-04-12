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
        X,
        Y
    };

    Axis GetComplementAxis(Axis axis)
    {
        return (axis == Axis::X) ? Axis::Y : Axis::X;
    }
}