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
        
        int m_Size = 8;

    public:
        Brush(const std::shared_ptr<ColorPalette>& colorPalette) : m_ColorPalette(colorPalette)
        {
        }

        virtual void Apply(std::shared_ptr<Layer> layer, const Vec2& position) = 0;
        virtual void Stroke(std::shared_ptr<Layer> layer, const Vec2& start, const Vec2& end) = 0;

        void SetSize(int size)
        {
            m_Size = size;
        }

        int GetSize() const
        {
            return m_Size;
        }

        std::shared_ptr<ColorPalette> GetColorPalette() const
        {
            return m_ColorPalette;
        }
    };

    class PencilBrush : public Brush
    {
    public:
        PencilBrush(std::shared_ptr<ColorPalette> colorPalette) : Brush(colorPalette)
        {
        }

        void Apply(std::shared_ptr<Layer> layer, const Vec2& position) override
        {
            ColorRGBA color = GetColorPalette()->GetGlobalColor();

            for (int x = -GetSize(); x <= GetSize(); ++x)
            {
                for (int y = -GetSize(); y <= GetSize(); ++y)
                {
                    if (x * x + y * y <= GetSize() * GetSize())
                    {
                        int pixelX = position.X + x;
                        int pixelY = position.Y + y;

                        layer->SetPixelColor(pixelX, pixelY, color);
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
    };
}