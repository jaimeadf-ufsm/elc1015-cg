#pragma once

#include <functional>
#include <memory>

#include "Vec2.h"
#include "Bitmap.h"

namespace yap
{
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

        void Scale(const Vec2& newSize, ScalingMethod method = ScalingMethod::NearestNeighbor)
        {
            Scale(static_cast<int>(newSize.X), static_cast<int>(newSize.Y), method);
        }

        void Scale(float newWidth, float newHeight, ScalingMethod method = ScalingMethod::NearestNeighbor)
        {
            std::shared_ptr<Bitmap> outputBitmap = std::make_shared<Bitmap>(static_cast<int>(newWidth), static_cast<int>(newHeight));

            Bitmap::Scale(*m_Bitmap, *outputBitmap, method);

            m_Bitmap = outputBitmap;
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