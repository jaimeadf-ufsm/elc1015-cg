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
        int m_Id = 0;

        int m_X = 0;
        int m_Y = 0;

        std::shared_ptr<Bitmap> m_Bitmap;

        bool m_Visible = true;

    public:
        Layer(int id, const Bitmap& bitmap)
            : m_Id(id), m_X(0), m_Y(0), m_Bitmap(std::make_shared<Bitmap>(bitmap))
        {
        }

        int GetId() const
        {
            return m_Id;
        }

        void SetPixel(int x, int y, const ColorRGBA& color)
        {
            int bitmapX = x - m_X;
            int bitmapY = y - m_Y;

            if (bitmapX >= 0 && bitmapX < m_Bitmap->GetWidth() && bitmapY >= 0 && bitmapY < m_Bitmap->GetHeight())
            {
                m_Bitmap->SetPixel(bitmapX, bitmapY, color);
            }
        }

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

        void SetPosition(const Vec2& position)
        {
            m_X = position.X;
            m_Y = position.Y;
        }

        Vec2 GetPosition() const
        {
            return Vec2(m_X, m_Y);
        }

        void FlipHorizontally()
        {
            m_Bitmap->FlipHorizontally();
        }

        void FlipVertically()
        {
            m_Bitmap->FlipVertically();
        }

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

        void Scale(const Vec2& newSize, ScalingMethod method = ScalingMethod::NearestNeighbor)
        {
            Scale(static_cast<int>(newSize.X), static_cast<int>(newSize.Y), method);
        }

        void Scale(float newWidth, float newHeight, ScalingMethod method = ScalingMethod::NearestNeighbor)
        {
            std::shared_ptr<Bitmap> output = std::make_shared<Bitmap>(static_cast<int>(newWidth), static_cast<int>(newHeight));

            Bitmap::Scale(*m_Bitmap, *output, method);

            m_Bitmap = output;
        }

        Vec2 GetSize() const
        {
            return Vec2(m_Bitmap->GetWidth(), m_Bitmap->GetHeight());
        }

        void SetVisible(bool visible)
        {
            m_Visible = visible;
        }

        bool IsVisible() const
        {
            return m_Visible;
        }

        void SetBitmap(const Bitmap& bitmap)
        {
            m_Bitmap = std::make_shared<Bitmap>(bitmap);
        }

        std::shared_ptr<const Bitmap> GetBitmap() const
        {
            return m_Bitmap;
        }
    };
}