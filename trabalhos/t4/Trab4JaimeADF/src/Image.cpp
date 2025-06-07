#include "Image.h"

Image::Image(std::size_t width, std::size_t height) :
    m_Width(width),
    m_Height(height),
    m_Pixels(width * height)
{

}

void Image::SetPixel(std::size_t x, std::size_t y, const Color& color)
{
    m_Pixels[y * m_Width + x] = color;
}

Color& Image::GetPixel(std::size_t x, std::size_t y)
{
    return m_Pixels[y * m_Width + x];
}

const Color& Image::GetPixel(std::size_t x, std::size_t y) const
{
    return m_Pixels[y * m_Width + x];
}

std::size_t Image::GetWidth() const
{
    return m_Width;
}

std::size_t Image::GetHeight() const
{
    return m_Height;
}

void Image::Resize(std::size_t newWidth, std::size_t newHeight)
{
    m_Width = newWidth;
    m_Height = newHeight;

    m_Pixels.resize(newWidth * newHeight);
}
