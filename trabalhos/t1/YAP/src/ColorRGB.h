#pragma once

namespace yap
{
    class ColorRGB
    {
    public:
        float R, G, B;

        ColorRGB() : R(0), G(0), B(0) {}
        ColorRGB(float r, float g, float b) : R(r), G(g), B(b) {}
        ColorRGB(float r, float g, float b, float a) : R(r), G(g), B(b) {}
        ColorRGB(const ColorRGB& other) : R(other.R), G(other.G), B(other.B) {}
    };
}