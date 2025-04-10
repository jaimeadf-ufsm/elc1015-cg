#pragma once

#include "Box.h"
#include "Text.h"

namespace yap
{
    class TextInput : public Box
    {
    private:
        std::string m_Value;

        std::shared_ptr<Text> m_Text;
        std::shared_ptr<Box> m_Cursor;
        

    public:
        std::function<void(TextInput&, const std::string&)> OnChange;
        std::function<void(TextInput&, const std::string&)> OnSubmit;
        std::function<void(TextInput&, const std::string&)> OnCancel;

        TextInput()
        {
            m_Text = std::make_shared<Text>();
            m_Cursor = std::make_shared<Box>();

            m_Cursor->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(1.0f), AxisSizingRule::Fixed(10.0f))
                    .WithBackground(BoxBackground::Solid(ColorRGB(255, 255, 255)))
                    .WithVisibility(false)
            );

            SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(24))
                    .WithAlignment(BoxAxisAlignment::Start, BoxAxisAlignment::Center)
                    .WithBackground(BoxBackground::Solid(ColorRGB(56, 56, 56)))
                    .WithForeground(ColorRGB(255, 255, 255))
                    .WithPadding(BoxPadding(4, 0))
            );

            SetStyle(
                ":focus",
                StyleSheet()
                    .WithBorder(BoxBorder::Solid(ColorRGB(74, 80, 124), 1.0f))
            );

            OnFocus = [this](Element& element)
            {
                m_Cursor->SetStyle(
                    m_Cursor->GetStyle()
                        .WithVisibility(true)
                );
            };

            OnUnfocus = [this](Element& element)
            {
                m_Cursor->SetStyle(
                    m_Cursor->GetStyle()
                        .WithVisibility(false)
                );
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
                    Unfocus();

                    if (OnSubmit)
                    {
                        OnSubmit(*this, m_Value);
                    }
                }
                else if (key == 27) // Escape
                {
                    Unfocus();

                    if (OnCancel)
                    {
                        OnCancel(*this, m_Value);
                    }
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