#pragma once

namespace yap
{
    class ColorRGBA
    {
    public:
        float R, G, B, A;

        ColorRGBA() : R(0), G(0), B(0), A(0) {}
        ColorRGBA(float r, float g, float b) : R(r), G(g), B(b), A(0.0f) {}
        ColorRGBA(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
        ColorRGBA(const ColorRGBA& other) : R(other.R), G(other.G), B(other.B), A(other.A) {}
    };
}