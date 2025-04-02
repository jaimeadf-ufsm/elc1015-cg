#pragma once

#include <vector>

#include "Color.h"

namespace yap
{
    class Bitmap
    {
    private:
        int m_Width;
        int m_Height;

        std::vector<ColorRGBA> m_Pixels;
    
    public:
        Bitmap(int width, int height) : m_Width(width), m_Height(height), m_Pixels(width * height, ColorRGBA(0, 0, 0, 0))
        {
        }

        void SetPixel(int x, int y, const ColorRGBA& color)
        {
            if (x >= 0 && x < m_Width && y >= 0 && y < m_Height)
            {
                m_Pixels[y * m_Width + x] = color;
            }
        }

        ColorRGBA GetPixel(int x, int y) const
        {
            if (x >= 0 && x < m_Width && y >= 0 && y < m_Height)
            {
                return m_Pixels[y * m_Width + x];
            }

            return ColorRGBA(0, 0, 0, 0);
        }

        int GetWidth() const
        {
            return m_Width;
        }

        int GetHeight() const
        {
            return m_Height;
        }
    };
}