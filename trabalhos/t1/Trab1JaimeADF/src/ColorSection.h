#pragma once

#include "ColorPalette.h"
#include "ColorPicker.h"

/**
 * @file ColorSection.h
 * @brief Defines the ColorSection class, which provides a UI component for displaying and editing colors.
 * 
 * The ColorSection class is a graphical user interface (GUI) component that allows users to view and modify
 * colors using a color palette, a color picker, and text fields for RGBA values. It also includes a preview
 * of the selected color.
 */

namespace yap
{
    /**
     * @class ColorSection
     * @brief A UI component for managing and displaying color-related elements.
     * 
     * The ColorSection class provides a header with RGBA value displays, a color preview, and a body containing
     * a color picker. It synchronizes with a global color palette and updates its elements dynamically.
     */
    class ColorSection : public Box
    {
    private:
        ColorRGBA m_CurrentColor; ///< The currently selected color in RGBA format.

        std::shared_ptr<ColorPalette> m_ColorPalette; ///< Shared pointer to the global color palette.

        std::shared_ptr<Bitmap> m_PreviewBackground; ///< Bitmap used for the color preview background.

        std::shared_ptr<ColorPicker> m_ColorPicker; ///< Shared pointer to the color picker component.

        std::shared_ptr<Text> m_RedText;   ///< Text field for displaying the red (R) component.
        std::shared_ptr<Text> m_GreenText; ///< Text field for displaying the green (G) component.
        std::shared_ptr<Text> m_BlueText;  ///< Text field for displaying the blue (B) component.
        std::shared_ptr<Text> m_AlphaText; ///< Text field for displaying the alpha (A) component.

    public:
        /**
         * @brief Constructs a ColorSection object.
         * 
         * @param colorPalette A shared pointer to the global color palette.
         */
        ColorSection(const std::shared_ptr<ColorPalette>& colorPalette)
            : m_ColorPalette(colorPalette)
        {
            m_PreviewBackground = std::make_shared<Bitmap>(40, 40);

            InitHeader();
            InitBody();

            SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(256))
                    .WithDirection(BoxDirection::Column)
                    .WithPadding(BoxPadding(0, 8, 0, 0))
                    .WithBackground(BoxBackground::Solid(ColorRGB(44, 44, 44)))
            );

            OnAnimate = [this](Element& element)
            {
                if (m_CurrentColor != m_ColorPalette->GetGlobalColor())
                {
                    m_CurrentColor = m_ColorPalette->GetGlobalColor();

                    RefreshPreview();
                    RefreshTexts();
                    RefreshColorPicker();
                }
            };
        }

        /**
         * @brief Initializes the header section of the ColorSection.
         * 
         * The header contains the color preview and RGBA value displays.
         */
        void InitHeader()
        {
            auto header = std::make_shared<Box>();
            auto preview = std::make_shared<Box>();

            header->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithPadding(BoxPadding(8, 8, 0, 8))
                    .WithGap(8)
            );

            preview->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(40), AxisSizingRule::Fixed(40))
                    .WithBackground(BoxBackground::Image(m_PreviewBackground))
                    .WithBackgroundReference(
                        BoxBackgroundTransparencyReference::Checkerboard(
                            ColorRGB(230, 230, 230),
                            ColorRGB(255, 255, 255),
                            8
                        )
                    )
            );

            header->AddChild(preview);
            header->AddChild(CreateColorValue(m_RedText, "R:"));
            header->AddChild(CreateColorValue(m_GreenText, "G:"));
            header->AddChild(CreateColorValue(m_BlueText, "B:"));
            header->AddChild(CreateColorValue(m_AlphaText, "A:"));

            AddChild(header);
        }

        /**
         * @brief Initializes the body section of the ColorSection.
         * 
         * The body contains the color picker component.
         */
        void InitBody()
        {
            auto body = std::make_shared<Box>();
            m_ColorPicker = std::make_shared<ColorPicker>();

            body->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
            );

            m_ColorPicker->OnChange = [this](ColorPicker& picker, const ColorHSVA& color)
            {
                m_ColorPalette->SetGlobalColor(color.ToRGBA());
            };

            body->AddChild(m_ColorPicker);

            AddChild(body);
        }

        /**
         * @brief Creates a container for displaying a color value.
         * 
         * @param value A reference to the text field that will display the value.
         * @param title The title of the color value (e.g., "R:", "G:", "B:", "A:").
         * @return A shared pointer to the created container.
         */
        std::shared_ptr<Box> CreateColorValue(std::shared_ptr<Text>& value, const std::string& title)
        {
            auto container = std::make_shared<Box>();

            auto label = std::make_shared<Text>();
            value = std::make_shared<Text>();

            container->SetStyle(
                StyleSheet()
                    .WithDirection(BoxDirection::Column)
                    .WithGap(4)
                    .WithForeground(ColorRGB(255, 255, 255))
            );

            label->Content = title;

            container->AddChild(label);
            container->AddChild(value);

            return container;
        }

        /**
         * @brief Refreshes the color preview to reflect the current color.
         */
        void RefreshPreview()
        {
            m_PreviewBackground->Clear(m_CurrentColor);
        }

        /**
         * @brief Updates the RGBA text fields to display the current color values.
         */
        void RefreshTexts()
        {
            m_RedText->Content = std::to_string(static_cast<int>(m_CurrentColor.R * 255.0f));
            m_GreenText->Content = std::to_string(static_cast<int>(m_CurrentColor.G * 255.0f));
            m_BlueText->Content = std::to_string(static_cast<int>(m_CurrentColor.B * 255.0f));
            m_AlphaText->Content = std::to_string(static_cast<int>(m_CurrentColor.A * 255.0f));
        }

        /**
         * @brief Synchronizes the color picker with the current color.
         */
        void RefreshColorPicker()
        {
            if (m_CurrentColor != m_ColorPicker->GetColor().ToRGBA())
            {
                m_ColorPicker->SetColor(m_CurrentColor.ToHSVA());
            }
        }
    };
}
