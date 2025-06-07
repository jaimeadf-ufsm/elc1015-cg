#include "Color.h"

Color::Color() : Color(0.0f, 0.0f, 0.0f)
{
}

Color::Color(int color) :
    Color(color >> 16 & 0xFF, color >> 8 & 0xFF, color & 0xFF)
{
}

Color::Color(int r, int g, int b) :
    Color(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f)
{
}

Color::Color(float r, float g, float b) : R(r), G(g), B(b)
{
}