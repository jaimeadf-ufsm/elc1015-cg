#pragma once

#include "Axis.h"

/**
 * @file BoxAlignment.h
 * @brief Defines the BoxAlignment structure and related enumerations for aligning elements along horizontal and vertical axes.
 */

namespace yap
{
    /**
     * @enum BoxAxisAlignment
     * @brief Specifies alignment options for a single axis.
     * 
     * - Start: Aligns elements to the start of the axis.
     * - Center: Aligns elements to the center of the axis.
     * - End: Aligns elements to the end of the axis.
     */
    enum class BoxAxisAlignment
    {
        Start,  ///< Align to the start of the axis.
        Center, ///< Align to the center of the axis.
        End     ///< Align to the end of the axis.
    };

    /**
     * @struct BoxAlignment
     * @brief Represents alignment settings for both horizontal and vertical axes.
     */
    struct BoxAlignment
    {
        BoxAxisAlignment Horizontal; ///< Alignment along the horizontal axis.
        BoxAxisAlignment Vertical;   ///< Alignment along the vertical axis.

        /**
         * @brief Default constructor.
         * 
         * Initializes both horizontal and vertical alignments to `BoxAxisAlignment::Start`.
         */
        BoxAlignment() :
            Horizontal(BoxAxisAlignment::Start), Vertical(BoxAxisAlignment::Start) {}

        /**
         * @brief Parameterized constructor.
         * 
         * @param horizontal Alignment for the horizontal axis.
         * @param vertical Alignment for the vertical axis.
         */
        BoxAlignment(BoxAxisAlignment horizontal, BoxAxisAlignment vertical) :
            Horizontal(horizontal), Vertical(vertical) {}

        /**
         * @brief Retrieves the alignment for a specified axis.
         * 
         * @param axis The axis (horizontal or vertical) for which to retrieve the alignment.
         * @return The alignment along the specified axis.
         */
        BoxAxisAlignment GetAlignmentAlongAxis(Axis axis)
        {
            return (axis == Axis::X ? Horizontal : Vertical);
        }
    };
}