#pragma once

#include <string>

#include "Color.h"
#include "Vector2.h"
#include "PolyLine.h"
#include "Image.h"

class Graphics {
public:
    static void StrokeLine(const ColorRGB& color, const PolyLine& polyline, float width = 1.0f);

    static void FillRectangle(const ColorRGB& color, const Vector2& position, const Vector2& size);
    static void FillCircle(const ColorRGB& color, const Vector2& center, float radius);

    static void DrawImage(const Image& image, const Vector2& position);
    static void DrawString(const ColorRGB& color, const Vector2& position, const std::string& text);

private:
    static std::vector<float> s_VerticesX;
    static std::vector<float> s_VerticesY;
};