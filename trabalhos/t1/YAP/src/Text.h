#pragma once

#include <string>

#include "Element.h"

namespace yap
{
    class Text : public Element
    {
    public:
        std::string Content;

        Text(const std::string& content = "")
            : Content(content)
        {
        }

        void ComputeIndependentDimensions() override
        {
            Element::ComputeIndependentDimensions();

            if (ComputedStyle.Size.Width.IsFit())
            {
                Size.X = MeasureTextWidth();
            }

            if (ComputedStyle.Size.Height.IsFit())
            {
                Size.Y = MeasureTextHeight();
            }
        }

        void Draw(RenderingContext& context) override
        {
            if (!ComputedStyle.Visibility)
            {
                return;
            }

            context.Color(ComputedStyle.Foreground);
            context.Text(Position + Vec2(0.0f, MeasureTextHeight()), Content.c_str());
        }
    
    private:
        float MeasureTextWidth()
        {
            return Content.size() * 10.0f;
        }

        float MeasureTextHeight()
        {
            return 13.0f;
        }
    };
}