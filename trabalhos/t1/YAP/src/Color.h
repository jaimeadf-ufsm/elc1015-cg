#pragma once

namespace yap
{
    class ColorRGB
    {
    public:
        float R, G, B;

        ColorRGB() : R(0), G(0), B(0) {}
        ColorRGB(float r, float g, float b) : R(r), G(g), B(b) {}
        ColorRGB(int r, int g, int b) : R(r / 255.0f), G(g / 255.0f), B(b / 255.0f) {}
    };

    class ColorRGBA
    {
    public:
        float R, G, B, A;

        ColorRGBA() : R(0), G(0), B(0), A(1.0f) {}
        ColorRGBA(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
        ColorRGBA(float r, float g, float b) : R(r), G(g), B(b), A(1.0f) {}
        ColorRGBA(int r, int g, int b, int a) : R(r / 255.0f), G(g / 255.0f), B(b / 255.0f), A(a / 255.0f) {}
        ColorRGBA(int r, int g, int b) : R(r / 255.0f), G(g / 255.0f), B(b / 255.0f), A(1.0f) {}
        ColorRGBA(const ColorRGB& rgb) : R(rgb.R), G(rgb.G), B(rgb.B), A(1.0f) {}
        ColorRGBA(const ColorRGB& rgb, float a) : R(rgb.R), G(rgb.G), B(rgb.B), A(a) {}

        ColorRGB Blend(const ColorRGB& other) const
        {
            return ColorRGB(
                R * (1 - A) + other.R * A,
                G * (1 - A) + other.G * A,
                B * (1 - A) + other.B * A
            );
        }

        ColorRGB ToRGB() const
        {
            return ColorRGB(R, G, B);
        }
    };

}