#pragma once

#include "Box.h"
#include "Text.h"

namespace yap
{
    class Modal : public Box
    {
    public:
        std::function<void(Modal &modal)> OnClose;

        Modal()
        {
            SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(512), AxisSizingRule::Fit())
                    .WithDirection(BoxDirection::Column)
                    .WithPadding(BoxPadding(1))
                    .WithGap(1)
                    .WithForeground(ColorRGB(255, 255, 255))
                    .WithBackground(BoxBackground::Solid(ColorRGB(70, 70, 70)))
            );

            OnKeyboardDown = [this](Element& element, KeyboardKey key)
            {
                if (key == 27) // Escape
                {
                    Close();
                }
            };
        }

        void Close()
        {
            if (OnClose)
            {
                OnClose(*this);
            }
        }
    
    protected:
        std::shared_ptr<Box> CreateHeader(const std::string& title)
        {
            auto header = std::make_shared<Box>();

            auto text = std::make_shared<Text>();
            auto fill = std::make_shared<Box>();

            auto closeButton = CreateIconButton(std::make_shared<Bitmap>(BMP::Load("YAP/assets/x-40x40.bmp")));

            text->Content = title;

            fill->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
            );

            closeButton->OnMousePress = [this](Element& element)
            {
                Close();
            };

            header->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithAlignment(BoxAxisAlignment::Start, BoxAxisAlignment::Center)
                    .WithPadding(BoxPadding(8, 8, 8, 16))
                    .WithBackground(BoxBackground::Solid(ColorRGB(44, 44, 44)))
            );

            header->AddChild(text);
            header->AddChild(fill);
            header->AddChild(closeButton);

            return header;
        }

        std::shared_ptr<Box> CreateBody()
        {
            auto body = std::make_shared<Box>();

            body->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithDirection(BoxDirection::Column)
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center)
                    .WithPadding(BoxPadding(8))
                    .WithGap(16)
                    .WithBackground(BoxBackground::Solid(ColorRGB(44, 44, 44)))
            );

            return body;
        }

        std::shared_ptr<Box> CreateIconButton(const std::shared_ptr<Bitmap>& icon)
        {
            auto button = std::make_shared<Box>();

            button->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(40), AxisSizingRule::Fixed(40))
                    .WithBackground(BoxBackground::Image(icon))
                    .WithBackgroundReference(BoxBackgroundTransparencyReference::Static(ColorRGB(44, 44, 44)))
            );

            button->SetStyle(
                ":hover",
                StyleSheet()
                    .WithBackgroundReference(BoxBackgroundTransparencyReference::Static(ColorRGB(56, 56, 56)))
            );

            return button;
        }

        std::shared_ptr<Box> CreateTextButton(const std::string& text)
        {
            auto button = std::make_shared<Box>();
            auto label = std::make_shared<Text>();

            label->Content = text;

            button->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(40))
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center)
                    .WithPadding(BoxPadding(8))
            );

            button->SetStyle(
                ":hover",
                StyleSheet()
                    .WithBackground(BoxBackground::Solid(ColorRGB(56, 56, 56)))
            );

            button->AddChild(label);

            return button;
        }
    };
}