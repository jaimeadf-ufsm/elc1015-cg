#pragma once

#include <chrono>

#include "Box.h"
#include "Text.h"

/**
 * @file TextInput.h
 * @brief Defines the TextInput class, a UI component for text input with cursor and event handling.
 */

namespace yap
{
    /**
     * @class TextInput
     * @brief A UI component for text input, supporting cursor visibility, keyboard input, and event handling.
     * 
     * The TextInput class extends the Box class and provides functionality for handling user input,
     * displaying a cursor, and triggering events such as OnChange, OnSubmit, OnCancel, and OnLeave.
     */
    class TextInput : public Box
    {
    private:
        std::string m_Value;

        std::shared_ptr<Text> m_Text;
        std::shared_ptr<Box> m_Cursor;
        

    public:
        std::function<void(TextInput&, const std::string&)> OnChange;
        std::function<void(TextInput&)> OnSubmit;
        std::function<void(TextInput&)> OnCancel;
        std::function<void(TextInput&)> OnLeave;

        TextInput()
        {
            m_Text = std::make_shared<Text>();
            m_Cursor = std::make_shared<Box>();

            m_Cursor->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(1.0f), AxisSizingRule::Fixed(12.0f))
                    .WithBackground(BoxBackground::Solid(ColorRGB(255, 255, 255)))
                    .WithVisibility(false)
            );

            SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(24))
                    .WithAlignment(BoxAxisAlignment::Start, BoxAxisAlignment::Center)
                    .WithBackground(BoxBackground::Solid(ColorRGB(30, 30, 30)))
                    .WithForeground(ColorRGB(255, 255, 255))
                    .WithBorder(BoxBorder::Solid(ColorRGB(68, 68, 68), 1.0f))
                    .WithPadding(BoxPadding(8, 0))
            );

            SetStyle(
                ":focus",
                StyleSheet()
                    .WithBorder(BoxBorder::Solid(ColorRGB(255, 255, 255), 1.0f))
            );

            OnAnimate = [this](Element& element)
            {
                if (IsFocused())
                {
                    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

                    m_Cursor->SetStyle(
                        m_Cursor->GetStyle()
                            .WithVisibility(ms % 1000 < 500)
                    );
                }
                else
                {
                    m_Cursor->SetStyle(
                        m_Cursor->GetStyle()
                            .WithVisibility(false)
                    );
                }
            };

            OnUnfocus = [this](Element& element)
            {
                if (OnLeave)
                {
                    OnLeave(*this);
                }
            };

            OnKeyboardDown = [this](Element& element, KeyboardKey key)
            {
                printf("Key: %d\n", key);

                if (key > 31 && key < 127)
                {
                    m_Value += static_cast<char>(key);

                    RefreshText();

                    if (OnChange)
                    {
                        OnChange(*this, m_Value);
                    }
                }
                else if (key == 8) // backspace
                {
                    if (!m_Value.empty())
                    {
                        m_Value.pop_back();

                        RefreshText();

                        if (OnChange)
                        {
                            OnChange(*this, m_Value);
                        }
                    }
                }
                else if (key == 13) // Enter
                {
                    if (OnSubmit)
                    {
                        OnSubmit(*this);
                    }

                    Unfocus();
                }
                else if (key == 27) // Escape
                {
                    if (OnCancel)
                    {
                        OnCancel(*this);
                    }

                    Unfocus();
                }

                RefreshText();
            };

            AddChild(m_Text);
            AddChild(m_Cursor);
        }

        void SetValue(const std::string& value)
        {
            m_Value = value;

            RefreshText();

            if (OnChange)
            {
                OnChange(*this, m_Value);
            }
        }

        const std::string& GetValue()
        {
            return m_Value;
        }
    
    private:
        void RefreshText()
        {
            m_Text->Content = m_Value;
        }
    };
}