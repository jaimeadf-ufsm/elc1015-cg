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
        NearestNeighbor, ///< Uses the nearest neighbor algorithm for scaling.
        Bilinear         ///< Uses bilinear interpolation for scaling.
    };

    /**
     * @class Bitmap
     * @brief Represents a 2D image with pixel manipulation capabilities.
     */
    class Bitmap
    {
    private:
        int m_Width; ///< The width of the bitmap.
        int m_Height; ///< The height of the bitmap.
        std::vector<ColorRGBA> m_Pixels; ///< The pixel data of the bitmap.

    public:
        /**
         * @brief Default constructor. Creates an empty bitmap.
         */
        Bitmap() : Bitmap(0, 0)
        {
        }

        /**
         * @brief Constructs a bitmap with the specified width and height.
         * @param width The width of the bitmap.
         * @param height The height of the bitmap.
         */
        Bitmap(int width, int height) : m_Width(width), m_Height(height), m_Pixels(width * height, ColorRGBA(0, 0, 0, 0))
        {
        }

        /**
         * @brief Flips the bitmap horizontally.
         */
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

        /**
         * @brief Flips the bitmap vertically.
         */
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

        /**
         * @brief Clears the bitmap by filling it with a specified color.
         * @param color The color to fill the bitmap with. Defaults to transparent black.
         */
        void Clear(const ColorRGBA& color = ColorRGBA(0, 0, 0, 0))
        {
            std::fill(m_Pixels.begin(), m_Pixels.end(), color);
        }

        /**
         * @brief Resizes the bitmap to the specified dimensions.
         * @param width The new width of the bitmap.
         * @param height The new height of the bitmap.
         */
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

        /**
         * @brief Sets the color of a specific pixel.
         * @param x The x-coordinate of the pixel.
         * @param y The y-coordinate of the pixel.
         * @param color The color to set the pixel to.
         */
        void SetPixel(int x, int y, const ColorRGBA& color)
        {
            m_Pixels[y * m_Width + x] = ColorRGBA::Clamp(color);
        }

        /**
         * @brief Gets the color of a specific pixel.
         * @param x The x-coordinate of the pixel.
         * @param y The y-coordinate of the pixel.
         * @return The color of the specified pixel.
         */
        const ColorRGBA &GetPixel(int x, int y) const
        {
            return m_Pixels[y * m_Width + x];
        }

        /**
         * @brief Gets the width of the bitmap.
         * @return The width of the bitmap.
         */
        int GetWidth() const
        {
            return m_Width;
        }

        /**
         * @brief Gets the height of the bitmap.
         * @return The height of the bitmap.
         */
        int GetHeight() const
        {
            return m_Height;
        }

        /**
         * @brief Rotates the source bitmap and stores the result in the destination bitmap.
         * @param source The source bitmap to rotate.
         * @param destination The destination bitmap to store the rotated image.
         * @param radians The angle of rotation in radians.
         * @param pivot The pivot point for rotation.
         * @param offset The offset to apply to the destination bitmap.
         */
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

        /**
         * @brief Scales the source bitmap and stores the result in the destination bitmap.
         * @param source The source bitmap to scale.
         * @param destination The destination bitmap to store the scaled image.
         * @param method The scaling method to use. Defaults to NearestNeighbor.
         */
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
        /**
         * @brief Scales the source bitmap using the nearest neighbor algorithm.
         * @param source The source bitmap to scale.
         * @param destination The destination bitmap to store the scaled image.
         */
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

        /**
         * @brief Scales the source bitmap using bilinear interpolation.
         * @param source The source bitmap to scale.
         * @param destination The destination bitmap to store the scaled image.
         */
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