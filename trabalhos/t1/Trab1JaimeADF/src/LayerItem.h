#pragma once

#include "Box.h"
#include "Text.h"
#include "Checkbox.h"

namespace yap
{
    class LayerItem : public Box
    {
    private:
        std::shared_ptr<Project> m_Project;
        std::shared_ptr<Layer> m_Layer;

        std::shared_ptr<Box> m_Container;

        std::shared_ptr<Checkbox> m_Checkbox;
        std::shared_ptr<Box> m_Divider;

        std::shared_ptr<Box> m_Information;

        std::shared_ptr<Box> m_Preview;
        std::shared_ptr<Text> m_Name;

        std::shared_ptr<Box> m_Line;

    public:
        LayerItem(std::shared_ptr<Project> project, std::shared_ptr<Layer> layer) : m_Project(project), m_Layer(layer)
        {
            m_Container = std::make_shared<Box>();

            m_Checkbox = std::make_shared<Checkbox>();
            m_Divider = std::make_shared<Box>();

            m_Information = std::make_shared<Box>();
            m_Preview = std::make_shared<Box>();
            m_Name = std::make_shared<Text>();

            m_Line = std::make_shared<Box>();
        
            SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithDirection(BoxDirection::Column)
            );

            m_Container->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithAlignment(BoxAxisAlignment::Start, BoxAxisAlignment::Center)
                    .WithPadding(BoxPadding(0, 0, 0, 8))
                    .WithGap(8)
            );

            m_Checkbox->SetChecked(m_Layer->IsVisible());

            m_Divider->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(1), AxisSizingRule::Fill())
                    .WithBackground(BoxBackground::Solid(ColorRGB(70, 70, 70)))
            );

            m_Information->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(48))
                    .WithAlignment(BoxAxisAlignment::Start, BoxAxisAlignment::Center)
                    .WithGap(8)
            );

            m_Information->SetStyle(
                "selected",
                StyleSheet()
                    .WithBackground(BoxBackground::Solid(ColorRGB(74, 80, 124)))
            );

            m_Preview->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(64), AxisSizingRule::Fixed(36))
                    .WithBackgroundSize(BoxBackgroundSizingRule::Contain())
                    .WithBackgroundPosition(BoxBackgroundPositioningRule::Center())
                    .WithBackgroundReference(
                        BoxBackgroundTransparencyReference::Checkerboard(
                            ColorRGB(230, 230, 230),
                            ColorRGB(255, 255, 255),
                            4
                        )
                    )
            );

            m_Line->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(1))
                    .WithBackground(BoxBackground::Solid(ColorRGB(70, 70, 70)))
            );

            m_Name->Content = "Layer #" + std::to_string(m_Layer->GetId());

            m_Information->AddChild(m_Divider);
            m_Information->AddChild(m_Preview);
            m_Information->AddChild(m_Name);

            m_Container->AddChild(m_Checkbox);
            m_Container->AddChild(m_Information);

            m_Checkbox->OnChange = [this](Checkbox& checkbox, bool checked)
            {
                m_Layer->SetVisible(checked);
            };

            m_Information->OnMousePress = [this](Element& element)
            {
                m_Project->SetActiveLayer(m_Layer);
            };

            AddChild(m_Container);
            AddChild(m_Line);
        }

        void Animate() override
        {
            Box::Animate();

            m_Information->ToggleTrait("selected", m_Project->GetActiveLayer() == m_Layer);
            m_Preview->SetStyle(
                m_Preview->GetStyle()
                    .WithBackground(BoxBackground::Image(m_Layer->GetBitmap()))
            );
        }
    };
}