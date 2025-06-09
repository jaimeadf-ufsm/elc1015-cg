#pragma once

#include <vector>

#include "Color.h"

struct Image
{
    Image();
    Image(std::size_t width, std::size_t height);

    void SetPixel(std::size_t x, std::size_t y, const ColorRGB& color);

    ColorRGB& GetPixel(std::size_t x, std::size_t y);
    const ColorRGB& GetPixel(std::size_t x, std::size_t y) const;

    std::size_t GetWidth() const;
    std::size_t GetHeight() const;
    
    void Resize(std::size_t newWidth, std::size_t newHeight);
    void Clear(const ColorRGB& color = ColorRGB(0, 0, 0));

private:
    std::size_t m_Width;
    std::size_t m_Height;

    std::vector<ColorRGB> m_Pixels;
};