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
        /**
         * @brief The text content to be rendered.
         */
        std::string Content;

        /**
         * @brief Constructs a Text object with optional initial content.
         * 
         * @param content The initial text content. Defaults to an empty string.
         */
        Text(const std::string& content = "")
            : Content(content)
        {
        }

        /**
         * @brief Computes the dimensions of the text element based on its content.
         * 
         * If the width or height is set to "fit" in the computed style, this method
         * calculates the dimensions based on the text content.
         */
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

        /**
         * @brief Draws the text element on the screen.
         * 
         * This method renders the text content at the specified position using the
         * rendering context. It respects the visibility and foreground color defined
         * in the computed style.
         * 
         * @param context The rendering context used for drawing.
         */
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
        /**
         * @brief Measures the width of the text content.
         * 
         * This method calculates the width of the text based on the number of characters
         * and a fixed character width.
         * 
         * @return The width of the text in pixels.
         */
        float MeasureTextWidth()
        {
            return Content.size() * 10.0f;
        }

        /**
         * @brief Measures the height of the text content.
         * 
         * This method returns a fixed height for the text.
         * 
         * @return The height of the text in pixels.
         */
        float MeasureTextHeight()
        {
            return 13.0f;
        }
    };
}