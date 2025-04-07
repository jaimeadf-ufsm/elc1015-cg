#pragma once

#include "Axis.h"

namespace yap
{
    enum class BoxAxisAlignment
    {
        Start,
        Center,
        End
    };

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