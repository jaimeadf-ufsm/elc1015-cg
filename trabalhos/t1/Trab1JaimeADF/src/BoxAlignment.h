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
        Start,
        Center,
        End
    };

    /**
     * @struct BoxAlignment
     * @brief Represents alignment settings for both horizontal and vertical axes.
     */
    struct BoxAlignment
    {
        BoxAxisAlignment Horizontal;
        BoxAxisAlignment Vertical;

        BoxAlignment() :
            Horizontal(BoxAxisAlignment::Start), Vertical(BoxAxisAlignment::Start) {}

        BoxAlignment(BoxAxisAlignment horizontal, BoxAxisAlignment vertical) :
            Horizontal(horizontal), Vertical(vertical) {}

        BoxAxisAlignment GetAlignmentAlongAxis(Axis axis)
        {
            return (axis == Axis::X ? Horizontal : Vertical);
        }
    };
}