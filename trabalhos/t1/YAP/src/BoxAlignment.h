#pragma once

#include "Axis.h"

namespace yap
{

    enum class LayoutAxisAlignment
    {
        Start,
        Center,
        End
    };

    struct BoxAlignment
    {
        LayoutAxisAlignment Horizontal;
        LayoutAxisAlignment Vertical;

        BoxAlignment() :
            Horizontal(LayoutAxisAlignment::Start), Vertical(LayoutAxisAlignment::Start) {}

        BoxAlignment(LayoutAxisAlignment horizontal, LayoutAxisAlignment vertical) :
            Horizontal(horizontal), Vertical(vertical) {}

        LayoutAxisAlignment GetAlignmentAlongAxis(Axis axis)
        {
            return (axis == Axis::X ? Horizontal : Vertical);
        }
    };
}