// Summary:
// This file defines the `Checkbox` class, a UI component that extends the `Box` class.
// The `Checkbox` class represents a toggleable checkbox with customizable styles and behavior.
// It includes functionality for handling user interactions and triggering events when the checkbox state changes.

#pragma once

#include "Box.h"

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
        bool m_Checked = false; ///< Indicates whether the checkbox is currently checked.
        std::shared_ptr<Box> m_Checkmark; ///< A child `Box` representing the visual checkmark.

    public:
        /**
         * @brief Event triggered when the checkbox state changes.
         * 
         * This function is called whenever the checkbox is toggled. It provides a reference
         * to the `Checkbox` instance and the new checked state.
         */
        std::function<void(Checkbox&, bool)> OnChange = nullptr;

        /**
         * @brief Constructs a new `Checkbox` instance.
         * 
         * Initializes the checkbox with default styles, sets up the checkmark, and defines
         * behavior for mouse press events to toggle the checkbox state.
         */
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

        /**
         * @brief Sets the checked state of the checkbox.
         * 
         * @param checked The new checked state to set.
         * 
         * Updates the internal state and toggles the "checked" trait of the checkmark.
         */
        void SetChecked(bool checked)
        {
            m_Checked = checked;
            m_Checkmark->ToggleTrait("checked", m_Checked);
        }
        
        /**
         * @brief Gets the current checked state of the checkbox.
         * 
         * @return `true` if the checkbox is checked, `false` otherwise.
         */
        bool IsChecked() const
        {
            return m_Checked;
        }
    };
}