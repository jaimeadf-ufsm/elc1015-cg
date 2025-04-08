#pragma once

#include <vector>

#include "Color.h"

namespace yap
{
    enum class ScalingMethod
    {
        NearestNeighbor,
        Bilinear
    };

    class Bitmap
    {
    private:
        int m_Width;
        int m_Height;

        std::vector<ColorRGBA> m_Pixels;
    
    public:
        Bitmap() : Bitmap(0, 0)
        {
        }

        Bitmap(int width, int height) : m_Width(width), m_Height(height), m_Pixels(width * height, ColorRGBA(0, 0, 0, 0))
        {
        }

        void Clear(const ColorRGBA& color = ColorRGBA(0, 0, 0, 0))
        {
            std::fill(m_Pixels.begin(), m_Pixels.end(), color);
        }

        void Reallocate(int width, int height)
        {
            if (width == m_Width && height == m_Height)
            {
                return;
            }

            m_Width = width;
            m_Height = height;
            m_Pixels.resize(width * height);
        }

        void SetPixel(int x, int y, const ColorRGBA& color)
        {
            m_Pixels[y * m_Width + x] = color;
        }

        const ColorRGBA &GetPixel(int x, int y) const
        {
            return m_Pixels[y * m_Width + x];
        }

        int GetWidth() const
        {
            return m_Width;
        }

        int GetHeight() const
        {
            return m_Height;
        }
        
        static void Scale(const Bitmap& source, Bitmap& destination, ScalingMethod method = ScalingMethod::NearestNeighbor)
        {
            switch (method)
            {
                case ScalingMethod::NearestNeighbor:
                    ScaleNearestNeighbor(source, destination);
                    break;
                case ScalingMethod::Bilinear:
                    ScaleBilinear(source, destination);
                    break;
            }
        }
    
    private:
        static void ScaleNearestNeighbor(const Bitmap& source, Bitmap& destination)
        {
            if (source.GetWidth() == 0 || source.GetHeight() == 0)
            {
                destination.Clear();
                return;
            }

            float xRatio = static_cast<float>(source.GetWidth()) / destination.GetWidth();
            float yRatio = static_cast<float>(source.GetHeight()) / destination.GetHeight();
            
            for (int y = 0; y < destination.GetHeight(); ++y)
            {
                for (int x = 0; x < destination.GetWidth(); ++x)
                {
                    int sourceX = static_cast<int>(x * xRatio);
                    int sourceY = static_cast<int>(y * yRatio);

                    destination.SetPixel(x, y, source.GetPixel(sourceX, sourceY));
                }
            }
        }

        static void ScaleBilinear(const Bitmap& source, Bitmap& destination)
        {
            float xRatio = static_cast<float>(source.GetWidth()) / destination.GetWidth();
            float yRatio = static_cast<float>(source.GetHeight()) / destination.GetHeight();

            for (int y = 0; y < destination.GetHeight(); ++y)
            {
                for (int x = 0; x < destination.GetWidth(); ++x)
                {
                    float srcX = x * xRatio;
                    float srcY = y * yRatio;

                    int x1 = static_cast<int>(srcX);
                    int y1 = static_cast<int>(srcY);
                    int x2 = std::min(x1 + 1, source.GetWidth() - 1);
                    int y2 = std::min(y1 + 1, source.GetHeight() - 1);

                    float dx = srcX - x1;
                    float dy = srcY - y1;

                    ColorRGBA c00 = source.GetPixel(x1, y1);
                    ColorRGBA c10 = source.GetPixel(x2, y1);
                    ColorRGBA c01 = source.GetPixel(x1, y2);
                    ColorRGBA c11 = source.GetPixel(x2, y2);

                    ColorRGBA top = ColorRGBA::Lerp(c00, c10, dx);
                    ColorRGBA bottom = ColorRGBA::Lerp(c01, c11, dx);
                    ColorRGBA finalColor = ColorRGBA::Lerp(top, bottom, dy);

                    destination.SetPixel(x, y, finalColor);
                }
            }
        }
    };
}