#pragma once

#include <vector>

#include "Math.h"
#include "Color.h"
#include "Vec2.h"

/**
 * @file Bitmap.h
 * @brief Provides the Bitmap class for image manipulation, including operations like flipping, scaling, and rotating.
 */

namespace yap
{
    /**
     * @enum ScalingMethod
     * @brief Specifies the scaling method to be used when resizing a bitmap.
     */
    enum class ScalingMethod
    {
        NearestNeighbor,
        Bilinear
    };

    /**
     * @class Bitmap
     * @brief Represents a 2D image with pixel manipulation capabilities.
     */
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

        void FlipHorizontally()
        {
            for (int y = 0; y < m_Height; ++y)
            {
                for (int x = 0; x < m_Width / 2; ++x)
                {
                    std::swap(m_Pixels[y * m_Width + x], m_Pixels[y * m_Width + (m_Width - 1 - x)]);
                }
            }
        }

        void FlipVertically()
        {
            for (int y = 0; y < m_Height / 2; ++y)
            {
                for (int x = 0; x < m_Width; ++x)
                {
                    std::swap(m_Pixels[y * m_Width + x], m_Pixels[(m_Height - 1 - y) * m_Width + x]);
                }
            }
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
            m_Pixels[y * m_Width + x] = ColorRGBA::Clamp(color);
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

        static void Rotate(const Bitmap& source, Bitmap& destination, float radians, Vec2 pivot, Vec2 offset)
        {
            destination.Clear();
            
            for (int y = 0; y < destination.GetHeight(); ++y)
            {
                for (int x = 0; x < destination.GetWidth(); ++x)
                {
                    Vec2 sourcePosition = Vec2(x, y) - offset;
                    sourcePosition.Rotate(-radians, pivot);

                    int sourceX = static_cast<int>(sourcePosition.X);
                    int sourceY = static_cast<int>(sourcePosition.Y);

                    if (sourceX >= 0 && sourceX < source.GetWidth() && sourceY >= 0 && sourceY < source.GetHeight())
                    {
                        destination.SetPixel(x, y, source.GetPixel(sourceX, sourceY));
                    }
                }
            }
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