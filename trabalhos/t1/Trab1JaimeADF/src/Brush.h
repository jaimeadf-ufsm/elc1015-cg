#pragma once

#include "Vec2.h"

#include "Layer.h"
#include "ColorPalette.h"

namespace yap
{
    class Brush
    {
    private:
        std::shared_ptr<ColorPalette> m_ColorPalette;
        
        float m_Size = 8;

    public:
        Brush(const std::shared_ptr<ColorPalette>& colorPalette) : m_ColorPalette(colorPalette)
        {
        }

        virtual void Apply(std::shared_ptr<Layer> layer, const Vec2& position) = 0;
        virtual void Stroke(std::shared_ptr<Layer> layer, const Vec2& start, const Vec2& end) = 0;

        void SetSize(float size)
        {
            m_Size = size;
        }

        float GetSize() const
        {
            return m_Size;
        }

        std::shared_ptr<ColorPalette> GetColorPalette() const
        {
            return m_ColorPalette;
        }
    };

    enum class PencilShape
    {
        Circle,
        Square,
        Triangle
    };

    class PencilBrush : public Brush
    {
    private:
        PencilShape m_Shape = PencilShape::Circle;

    public:
        PencilBrush(std::shared_ptr<ColorPalette> colorPalette) : Brush(colorPalette)
        {
        }

        void Apply(std::shared_ptr<Layer> layer, const Vec2& position) override
        {
            ColorRGBA color = GetColorPalette()->GetGlobalColor();

            float size = GetSize();
            int halfSize = static_cast<int>(std::ceil(size / 2.0f));

            for (int y = -halfSize; y <= halfSize; ++y)
            {
                for (int x = -halfSize; x <= halfSize; ++x)
                {
                    if (IsInsideShape(x, y, size))
                    {
                        int pixelX = static_cast<int>(position.X) + x;
                        int pixelY = static_cast<int>(position.Y) + y;

                        layer->SetPixel(pixelX, pixelY, color);
                    }
                }
            }
        }

        void Stroke(std::shared_ptr<Layer> layer, const Vec2& start, const Vec2& end) override
        {
            Vec2 direction = end - start;
            float length = direction.Length();

            direction.Normalize();

            for (float t = 0; t <= length; t += 1.0f)
            {
                Vec2 position = start + direction * t;
                Apply(layer, position);
            }
        }

        void SetShape(PencilShape shape)
        {
            m_Shape = shape;
        }

        PencilShape GetShape() const
        {
            return m_Shape;
        }
    
    private:
        bool IsInsideShape(int x, int y, float size)
        {
            switch (m_Shape)
            {
                case PencilShape::Circle:
                    return IsInsideCircle(x, y, size);
                case PencilShape::Square:
                    return IsInsideSquare(x, y, size);
                case PencilShape::Triangle:
                    return IsInsideTriangle(x, y, size);
                default:
                    return false;
            }
        }

        bool IsInsideCircle(int x, int y, float size)
        {
            return x * x + y * y <= (size * size) / 4.0f;
        }

        bool IsInsideSquare(int x, int y, float size)
        {
            return std::abs(x) <= size / 2.0f && std::abs(y) <= size / 2.0f;
        }

        bool IsInsideTriangle(int x, int y, float size)
        {
            return y >= 0 && y <= size && x >= -y && x <= y;
        }
    };
}