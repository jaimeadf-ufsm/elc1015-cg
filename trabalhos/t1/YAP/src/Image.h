#pragma once

#include <memory>

#include "Element.h"
#include "Bitmap.h"

namespace yap
{
    class Image : public Element
    {
    public:
        std::shared_ptr<Bitmap> Source;

        Image() : Source(std::make_shared<Bitmap>(0, 0))
        {
        }

        void ComputeIndependentDimensions() override
        {
            Element::ComputeIndependentDimensions();

            if (Size.Width.IsFit())
            {
                ViewportSize.X = Source->GetWidth();
            }

            if (Size.Height.IsFit())
            {
                ViewportSize.Y = Source->GetHeight();
            }
        }

        void Draw(RenderingContext& context) override
        {
            for (int x = 0; x < Source->GetWidth(); x++)
            {
                for (int y = 0; y < Source->GetHeight(); y++)
                {
                    if (x >= ViewportSize.X || y >= ViewportSize.Y)
                    {
                        continue;
                    }

                    Vec2 position = ViewportPosition + Vec2(x, y);

                    context.Color(Source->GetPixel(x, y).ToRGB());
                    context.FillRectangle(position, Vec2(1, 1));
                }
            }
        }
    };
}