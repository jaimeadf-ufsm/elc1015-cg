#pragma once

#include "ColorPalette.h"
#include "ColorPicker.h"

namespace yap
{
    class ColorSection : public Box
    {
    private:
        ColorRGBA m_CurrentColor;

        std::shared_ptr<ColorPalette> m_ColorPalette;

        std::shared_ptr<Bitmap> m_PreviewBackground;

        std::shared_ptr<ColorPicker> m_ColorPicker;

        std::shared_ptr<Text> m_RedText;
        std::shared_ptr<Text> m_GreenText;
        std::shared_ptr<Text> m_BlueText;
        std::shared_ptr<Text> m_AlphaText;

    public:
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

        void RefreshPreview()
        {
            m_PreviewBackground->Clear(m_CurrentColor);
        }

        void RefreshTexts()
        {
            m_RedText->Content = std::to_string(static_cast<int>(m_CurrentColor.R * 255.0f));
            m_GreenText->Content = std::to_string(static_cast<int>(m_CurrentColor.G * 255.0f));
            m_BlueText->Content = std::to_string(static_cast<int>(m_CurrentColor.B * 255.0f));
            m_AlphaText->Content = std::to_string(static_cast<int>(m_CurrentColor.A * 255.0f));
        }

        void RefreshColorPicker()
        {
            if (m_CurrentColor != m_ColorPicker->GetColor().ToRGBA())
            {
                m_ColorPicker->SetColor(m_CurrentColor.ToHSVA());
            }
        }
    };
}
