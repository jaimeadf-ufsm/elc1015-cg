#pragma once

namespace yap
{
    template<typename T>
    T Clamp(const T& value, const T& min, const T& max)
    {
        return std::max(min, std::min(value, max));
    }

    float Lerp(float a, float b, float t)
    {
        return a + t * (b - a);
    }
};