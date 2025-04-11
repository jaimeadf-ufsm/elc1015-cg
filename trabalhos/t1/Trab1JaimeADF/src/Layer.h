#pragma once

#include <functional>
#include <memory>
#include <queue>

#include "Vec2.h"
#include "Bitmap.h"

/**
 * @file Layer.h
 * @brief Defines the Layer class, which represents a drawable layer with position, visibility, and bitmap manipulation capabilities.
 */

namespace yap
{
    /**
     * @class Layer
     * @brief Represents a drawable layer that can manipulate a bitmap, including transformations, pixel operations, and visibility control.
     */
    class Layer
    {
    private:
        int m_Id = 0; ///< Unique identifier for the layer.
        int m_X = 0; ///< X-coordinate of the layer's position.
        int m_Y = 0; ///< Y-coordinate of the layer's position.
        std::shared_ptr<Bitmap> m_Bitmap; ///< Shared pointer to the bitmap associated with the layer.
        bool m_Visible = true; ///< Visibility flag for the layer.

    public:
        /**
         * @brief Constructs a Layer with a given ID and bitmap.
         * @param id The unique identifier for the layer.
         * @param bitmap The bitmap associated with the layer.
         */
        Layer(int id, const Bitmap& bitmap)
            : m_Id(id), m_X(0), m_Y(0), m_Bitmap(std::make_shared<Bitmap>(bitmap))
        {
        }

        /**
         * @brief Gets the unique identifier of the layer.
         * @return The layer's ID.
         */
        int GetId() const
        {
            return m_Id;
        }

        /**
         * @brief Sets a pixel in the layer's bitmap at the specified position.
         * @param x The X-coordinate of the pixel.
         * @param y The Y-coordinate of the pixel.
         * @param color The color to set the pixel to.
         */
        void SetPixel(int x, int y, const ColorRGBA& color)
        {
            int bitmapX = x - m_X;
            int bitmapY = y - m_Y;

            if (bitmapX >= 0 && bitmapX < m_Bitmap->GetWidth() && bitmapY >= 0 && bitmapY < m_Bitmap->GetHeight())
            {
                m_Bitmap->SetPixel(bitmapX, bitmapY, color);
            }
        }

        /**
         * @brief Gets the color of a pixel in the layer's bitmap at the specified position.
         * @param x The X-coordinate of the pixel.
         * @param y The Y-coordinate of the pixel.
         * @return The color of the pixel.
         */
        ColorRGBA GetPixel(int x, int y) const
        {
            int bitmapX = x - m_X;
            int bitmapY = y - m_Y;

            if (bitmapX < 0 || bitmapX >= m_Bitmap->GetWidth() || bitmapY < 0 || bitmapY >= m_Bitmap->GetHeight())
            {
                return ColorRGBA(0, 0, 0, 0); 
            }

            return m_Bitmap->GetPixel(bitmapX, bitmapY);
        }

        /**
         * @brief Sets the position of the layer.
         * @param position The new position of the layer.
         */
        void SetPosition(const Vec2& position)
        {
            m_X = position.X;
            m_Y = position.Y;
        }

        /**
         * @brief Gets the position of the layer.
         * @return The position of the layer as a Vec2.
         */
        Vec2 GetPosition() const
        {
            return Vec2(m_X, m_Y);
        }

        /**
         * @brief Flips the layer's bitmap horizontally.
         */
        void FlipHorizontally()
        {
            m_Bitmap->FlipHorizontally();
        }

        /**
         * @brief Flips the layer's bitmap vertically.
         */
        void FlipVertically()
        {
            m_Bitmap->FlipVertically();
        }

        /**
         * @brief Fills a region of the layer's bitmap with a specified color using flood fill.
         * @param position The starting position for the fill.
         * @param color The color to fill the region with.
         */
        void Fill(Vec2 position, const ColorRGBA& color)
        {
            int x = static_cast<int>(position.X);
            int y = static_cast<int>(position.Y);

            int width = m_Bitmap->GetWidth();
            int height = m_Bitmap->GetHeight();

            ColorRGBA targetColor = GetPixel(x, y);

            if (targetColor == color)
            {
                return;
            }

            std::queue<std::pair<int, int>> q;

            SetPixel(x, y, color);
            q.push({x, y});

            int dx[4] = {-1, 1, 0, 0};
            int dy[4] = {0, 0, -1, 1};

            while (!q.empty()) {
                auto c = q.front();
                int cx = c.first;
                int cy = c.second;

                q.pop();

                for (int i = 0; i < 4; ++i) {
                    int nx = cx + dx[i];
                    int ny = cy + dy[i];

                    if (nx >= 0 && ny >= 0 && nx < width && ny < height) {
                        if (GetPixel(nx, ny) == targetColor)
                        {
                            SetPixel(nx, ny, color);
                            q.push({nx, ny});
                        }
                    }
                }
            }
        }

        /**
         * @brief Rotates the layer's bitmap around a pivot point.
         * @param angle The angle of rotation in degrees.
         * @param pivot The pivot point for the rotation (default is the origin).
         */
        void Rotate(float angle, const Vec2& pivot = Vec2())
        {
            Vec2 size = GetSize();
            Vec2 position = GetPosition();

            std::vector<Vec2> corners = {
                position,
                position + Vec2(size.X, 0.0f),
                position + Vec2(0.0f, size.Y),
                position + size
            };

            for (auto& point : corners)
            {
                point.Rotate(angle, pivot);
                point.Floor();
            }

            Vec2 newTopLeft = corners[0];
            Vec2 newBottomRight = corners[0];

            for (const auto& point : corners)
            {
                newTopLeft.X = std::min(newTopLeft.X, point.X);
                newTopLeft.Y = std::min(newTopLeft.Y, point.Y);
                newBottomRight.X = std::max(newBottomRight.X, point.X);
                newBottomRight.Y = std::max(newBottomRight.Y, point.Y);
            }

            Vec2 newSize = newBottomRight - newTopLeft;
            Vec2 newPosition = newTopLeft;

            std::shared_ptr<Bitmap> output = std::make_shared<Bitmap>(static_cast<int>(newSize.X), static_cast<int>(newSize.Y));

            Bitmap::Rotate(*m_Bitmap, *output, angle, pivot - position, position - newPosition);

            m_Bitmap = output;
            SetPosition(newPosition);
        }

        /**
         * @brief Scales the layer's bitmap to a new size.
         * @param newSize The new size as a Vec2.
         * @param method The scaling method (default is NearestNeighbor).
         */
        void Scale(const Vec2& newSize, ScalingMethod method = ScalingMethod::NearestNeighbor)
        {
            Scale(static_cast<int>(newSize.X), static_cast<int>(newSize.Y), method);
        }

        /**
         * @brief Scales the layer's bitmap to a new width and height.
         * @param newWidth The new width of the bitmap.
         * @param newHeight The new height of the bitmap.
         * @param method The scaling method (default is NearestNeighbor).
         */
        void Scale(float newWidth, float newHeight, ScalingMethod method = ScalingMethod::NearestNeighbor)
        {
            std::shared_ptr<Bitmap> output = std::make_shared<Bitmap>(static_cast<int>(newWidth), static_cast<int>(newHeight));

            Bitmap::Scale(*m_Bitmap, *output, method);

            m_Bitmap = output;
        }

        /**
         * @brief Gets the size of the layer's bitmap.
         * @return The size of the bitmap as a Vec2.
         */
        Vec2 GetSize() const
        {
            return Vec2(m_Bitmap->GetWidth(), m_Bitmap->GetHeight());
        }

        /**
         * @brief Sets the visibility of the layer.
         * @param visible True to make the layer visible, false to hide it.
         */
        void SetVisible(bool visible)
        {
            m_Visible = visible;
        }

        /**
         * @brief Checks if the layer is visible.
         * @return True if the layer is visible, false otherwise.
         */
        bool IsVisible() const
        {
            return m_Visible;
        }

        /**
         * @brief Sets a new bitmap for the layer.
         * @param bitmap The new bitmap to set.
         */
        void SetBitmap(const Bitmap& bitmap)
        {
            m_Bitmap = std::make_shared<Bitmap>(bitmap);
        }

        /**
         * @brief Gets the bitmap associated with the layer.
         * @return A shared pointer to the bitmap.
         */
        std::shared_ptr<const Bitmap> GetBitmap() const
        {
            return m_Bitmap;
        }
    };
}