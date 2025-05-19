#include "Color.h"

const ColorRGB ColorRGB::Black = ColorRGB(0.0f, 0.0f, 0.0f);
const ColorRGB ColorRGB::White = ColorRGB(1.0f, 1.0f, 1.0f);
const ColorRGB ColorRGB::Gray = ColorRGB(0.5f, 0.5f, 0.5f);
const ColorRGB ColorRGB::Red = ColorRGB(1.0f, 0.0f, 0.0f);
const ColorRGB ColorRGB::Green = ColorRGB(0.0f, 1.0f, 0.0f);
const ColorRGB ColorRGB::Blue = ColorRGB(0.0f, 0.0f, 1.0f);
const ColorRGB ColorRGB::Cyan = ColorRGB(0.0f, 1.0f, 1.0f);
const ColorRGB ColorRGB::Magenta = ColorRGB(1.0f, 0.0f, 1.0f);
const ColorRGB ColorRGB::Yellow = ColorRGB(1.0f, 1.0f, 0.0f);

ColorRGB::ColorRGB()
{
    R = 0.0f;
    G = 0.0f;
    B = 0.0f;
}

ColorRGB::ColorRGB(float r, float g, float b)
{
    R = r;
    G = g;
    B = b;
}

ColorRGB::ColorRGB(int r, int g, int b)
{
    R = static_cast<float>(r) / 255.0f;
    G = static_cast<float>(g) / 255.0f;
    B = static_cast<float>(b) / 255.0f;
}

ColorRGB::ColorRGB(int color)
{
    R = static_cast<float>((color >> 16) & 0XFF) / 255.0f;
    G = static_cast<float>((color >> 8) & 0XFF) / 255.0f;
    B = static_cast<float>(color & 0XFF) / 255.0f;
}