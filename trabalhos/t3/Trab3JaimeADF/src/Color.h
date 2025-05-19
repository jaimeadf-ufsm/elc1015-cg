#pragma once

struct ColorRGB
{
    static const ColorRGB Black;
    static const ColorRGB White;
    static const ColorRGB Gray;
    static const ColorRGB Red;
    static const ColorRGB Green;
    static const ColorRGB Blue;
    static const ColorRGB Cyan;
    static const ColorRGB Magenta;
    static const ColorRGB Yellow;

    float R;
    float G;
    float B;

    ColorRGB();
    ColorRGB(float r, float g, float b);
    ColorRGB(int r, int g, int b);
    ColorRGB(int color);
};