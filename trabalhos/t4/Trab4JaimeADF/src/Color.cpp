#include "Color.h"

#include "Vector3.h"
#include "Vector4.h"

ColorRGB::ColorRGB() : ColorRGB(0.0f, 0.0f, 0.0f)
{
}

ColorRGB::ColorRGB(int color) :
    ColorRGB(color >> 16 & 0xFF, color >> 8 & 0xFF, color & 0xFF)
{
}

ColorRGB::ColorRGB(int r, int g, int b) :
    ColorRGB(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f)
{
}

ColorRGB::ColorRGB(float r, float g, float b) : R(r), G(g), B(b)
{
}

ColorRGB::ColorRGB(const Vector3& vector) : R(vector.X), G(vector.Y), B(vector.Z)
{
}

ColorRGBA::ColorRGBA() : ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f)
{
}

ColorRGBA::ColorRGBA(int color) :
    ColorRGBA(color >> 16 & 0xFF, color >> 8 & 0xFF, color & 0xFF, color >> 24 & 0xFF)
{
}

ColorRGBA::ColorRGBA(int r, int g, int b, int a) :
    ColorRGBA(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f, static_cast<float>(a) / 255.0f)
{
}

ColorRGBA::ColorRGBA(float r, float g, float b, float a) : R(r), G(g), B(b), A(a)
{
}

ColorRGBA::ColorRGBA(const ColorRGB& rgb, float alpha) : R(rgb.R), G(rgb.G), B(rgb.B), A(alpha)
{
}

ColorRGBA::ColorRGBA(const Vector4& vector) : R(vector.X), G(vector.Y), B(vector.Z), A(vector.W)
{
}