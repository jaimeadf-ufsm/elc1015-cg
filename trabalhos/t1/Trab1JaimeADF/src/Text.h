#pragma once

#include <string>

#include "Element.h"

/**
 * @file Text.h
 * @brief Defines the Text class, which represents a text element in the rendering system.
 * 
 * The Text class is a specialized type of Element that handles rendering and measuring
 * text content. It provides methods for computing dimensions based on the text content
 * and drawing the text on the screen.
 */

namespace yap
{
    /**
     * @class Text
     * @brief Represents a text element in the rendering system.
     * 
     * The Text class extends the Element class and provides functionality for rendering
     * text content. It supports automatic dimension computation based on the text size
     * and allows customization of text content and style.
     */
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
            context.Text(Position + Vec2(0.0f, 11.0f), Content.c_str());
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