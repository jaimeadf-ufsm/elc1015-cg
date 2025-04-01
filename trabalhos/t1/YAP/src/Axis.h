#pragma once

namespace yap
{
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