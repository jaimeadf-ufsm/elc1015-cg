#pragma once

// Summary:
// This header file contains utility mathematical functions for common operations,
// such as clamping a value within a range and performing linear interpolation.

namespace yap
{
    /**
     * @brief Clamps a value within the specified range.
     * 
     * @tparam T The type of the value, minimum, and maximum.
     * @param value The value to clamp.
     * @param min The minimum allowable value.
     * @param max The maximum allowable value.
     * @return T The clamped value, constrained between min and max.
     */
    template<typename T>
    T Clamp(const T& value, const T& min, const T& max)
    {
        return std::max(min, std::min(value, max));
    }

    /**
     * @brief Performs linear interpolation between two values.
     * 
     * @param a The starting value.
     * @param b The ending value.
     * @param t The interpolation factor, typically between 0.0 and 1.0.
     * @return float The interpolated value.
     */
    float Lerp(float a, float b, float t)
    {
        return a + t * (b - a);
    }
};