#pragma once

#include "Box.h"

/**
 * @file Checkbox.h
 * @brief Defines the UI component to represent a toggleable checkbox.
 */

namespace yap
{
    /**
     * @class Checkbox
     * @brief A UI component representing a toggleable checkbox.
     * 
     * The `Checkbox` class extends the `Box` class and provides functionality for creating
     * a checkbox with customizable styles, handling user interactions, and triggering events
     * when the checkbox state changes.
     */
    class Checkbox : public Box
    {
    private:
        bool m_Checked = false;

        std::shared_ptr<Box> m_Checkmark;

    public:
        std::function<void(Checkbox&, bool)> OnChange = nullptr;

        Checkbox()
        {
            m_Checkmark = std::make_shared<Box>();

            m_Checkmark->SetStyle(
                StyleSheet()
                    .WithVisibility(false)
                    .WithSize(AxisSizingRule::Fixed(12), AxisSizingRule::Fixed(12))
                    .WithAlignment(BoxAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center))
                    .WithBackground(BoxBackground::Solid(ColorRGB(255, 255, 255)))
            );

            m_Checkmark->SetStyle(
                "checked",
                StyleSheet()
                    .WithVisibility(true)
            );

            SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(16), AxisSizingRule::Fixed(16))
                    .WithAlignment(BoxAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center))
                    .WithBackground(BoxBackground::Solid(ColorRGB(30, 30, 30)))
                    .WithBorder(BoxBorder::Solid(ColorRGB(68, 68, 68)))
            );

            SetStyle(
                ":hover",
                StyleSheet()
                    .WithBackground(BoxBackground::Solid(ColorRGB(50, 50, 50)))
            );

            OnMousePress = [this](Element& element) {
                SetChecked(!m_Checked);

                if (OnChange)
                {
                    OnChange(*this, m_Checked);
                }
            };

            AddChild(m_Checkmark);
        }

        void SetChecked(bool checked)
        {
            m_Checked = checked;
            m_Checkmark->ToggleTrait("checked", m_Checked);
        }
        
        bool IsChecked() const
        {
            return m_Checked;
        }
    };
}