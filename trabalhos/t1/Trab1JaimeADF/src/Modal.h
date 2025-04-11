// Summary:
// This file defines the `Modal` class, a UI component that extends the `Box` class to create a modal dialog.
// The modal includes customizable headers, bodies, and buttons, and supports keyboard and mouse interactions.

#pragma once

#include "Box.h"
#include "Text.h"

namespace yap
{
    /**
     * @class Modal
     * @brief Represents a modal dialog box with customizable header, body, and buttons.
     * 
     * The `Modal` class extends the `Box` class to provide a modal dialog with a header, body, and buttons.
     * It supports keyboard and mouse interactions, including closing the modal via the Escape key or a close button.
     */
    class Modal : public Box
    {
    public:
        /**
         * @brief Callback function triggered when the modal is closed.
         */
        std::function<void(Modal &modal)> OnClose;

        /**
         * @brief Constructs a `Modal` object with default styles and behavior.
         * 
         * Initializes the modal with a fixed width, dynamic height, and default styles.
         * Binds the Escape key to close the modal.
         */
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

        /**
         * @brief Closes the modal and triggers the `OnClose` callback if set.
         */
        void Close()
        {
            if (OnClose)
            {
                OnClose(*this);
            }
        }
    
    protected:
        /**
         * @brief Creates a header for the modal with a title and a close button.
         * 
         * @param title The title text to display in the header.
         * @return A shared pointer to the header `Box` object.
         */
        std::shared_ptr<Box> CreateHeader(const std::string& title)
        {
            auto header = std::make_shared<Box>();

            auto text = std::make_shared<Text>();
            auto fill = std::make_shared<Box>();

            auto closeButton = CreateIconButton(std::make_shared<Bitmap>(BMP::Load("Trab1JaimeADF/assets/x-40x40.bmp")));

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

        /**
         * @brief Creates the body of the modal.
         * 
         * @return A shared pointer to the body `Box` object.
         */
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

        /**
         * @brief Creates an icon button with a specified bitmap image.
         * 
         * @param icon A shared pointer to the `Bitmap` object representing the button's icon.
         * @return A shared pointer to the icon button `Box` object.
         */
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

        /**
         * @brief Creates a text button with a specified label.
         * 
         * @param text The label text to display on the button.
         * @return A shared pointer to the text button `Box` object.
         */
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