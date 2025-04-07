#pragma once

#include "Box.h"

namespace yap
{
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
    
    protected:
        void HandleMousePress(Mouse &mouse) override
        {
            Element::HandleMousePress(mouse);
            SetChecked(!m_Checked);

            if (OnChange)
            {
                OnChange(*this, m_Checked);
            }
        }
    };
}