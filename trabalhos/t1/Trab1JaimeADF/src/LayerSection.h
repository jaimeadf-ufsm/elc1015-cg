#pragma once

#include "BMP.h"

#include "Project.h"

#include "Box.h"
#include "Text.h"
#include "LayerItem.h"

/**
 * @file LayerSection.h
 * @brief Defines the LayerSection class, which represents a UI component for managing layers in a project.
 */

namespace yap
{
    /**
     * @class LayerItem
     * @brief A UI component representing a layer in a project.
     * 
     * The LayerItem class extends the Box class and provides a structured layout for displaying
     * layer-related information, such as visibility, name, and preview. It also handles user interactions
     * like toggling visibility and selecting the active layer.
     */
    class LayerSection : public Box
    {
    private:
        std::shared_ptr<Project> m_Project;

        std::shared_ptr<Box> m_Header;
        std::shared_ptr<Box> m_List;
        std::shared_ptr<Box> m_Controls;

    public:
        LayerSection(std::shared_ptr<Project> project) : m_Project(project)
        {
            m_Header = CreateHeader();
            m_List = CreateList();
            m_Controls = CreateControls();

            SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Start)
                    .WithDirection(BoxDirection::Column)
                    .WithGap(1)
                    .WithForeground(ColorRGB(255, 255, 255))
            );

            AddChild(m_Header);
            AddChild(m_List);
            AddChild(m_Controls);

            m_Project->OnLayerCreated = [this](Project& project, std::shared_ptr<Layer> layer)
            {
                RefreshLayers();
            };

            m_Project->OnLayerDeleted = [this](Project& project, std::shared_ptr<Layer> layer)
            {
                RefreshLayers();
            };

            m_Project->OnLayerMoved = [this](Project& project, std::shared_ptr<Layer> layer)
            {
                RefreshLayers();
            };

            RefreshLayers();
        }
    
    private:
        std::shared_ptr<Box> CreateHeader()
        {
            auto header = std::make_shared<Box>();

            header->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithBackground(BoxBackground::Solid(ColorRGB(44, 44, 44)))
                    .WithPadding(8)
            );

            header->AddChild(std::make_shared<Text>("Layers"));

            return header;
        }

        std::shared_ptr<Box> CreateList()
        {
            auto list = std::make_shared<Box>();

            list->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
                    .WithDirection(BoxDirection::Column)
                    .WithBackground(BoxBackground::Solid(ColorRGB(44, 44, 44)))
            );

            return list;
        }

        std::shared_ptr<Box> CreateControls()
        {
            auto controls = std::make_shared<Box>();

            controls->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center)
                    .WithBackground(BoxBackground::Solid(ColorRGB(44, 44, 44)))
                    .WithPadding(BoxPadding(8))
                    .WithGap(8)
            );

            controls->AddChild(CreateControl(
                std::make_shared<Bitmap>(BMP::Load("Trab1JaimeADF/assets/plus-40x40.bmp")),
                [this](Element& element)
                {
                    m_Project->CreateLayer();
                }
            ));

            controls->AddChild(CreateControl(
                std::make_shared<Bitmap>(BMP::Load("Trab1JaimeADF/assets/x-40x40.bmp")),
                [this](Element& element)
                {
                    m_Project->DeleteActiveLayer();
                }
            ));

            controls->AddChild(CreateControl(
                std::make_shared<Bitmap>(BMP::Load("Trab1JaimeADF/assets/chevron-up-40x40.bmp")),
                [this](Element& element)
                {
                    m_Project->MoveLayerUp(m_Project->GetActiveLayer());
                }
            ));
            
            controls->AddChild(CreateControl(
                std::make_shared<Bitmap>(BMP::Load("Trab1JaimeADF/assets/chevron-down-40x40.bmp")),
                [this](Element& element)
                {
                    m_Project->MoveLayerDown(m_Project->GetActiveLayer());
                }
            ));

            return controls;
        }

        std::shared_ptr<Box> CreateControl(std::shared_ptr<const Bitmap> bitmap, std::function<void(Element&)> action = nullptr)
        {
            auto control = std::make_shared<Box>();

            control->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(40), AxisSizingRule::Fixed(40))
                    .WithBackground(BoxBackground::Image(bitmap))
                    .WithBackgroundReference(BoxBackgroundTransparencyReference::Static(ColorRGB(44, 44, 44)))
            );

            control->SetStyle(
                ":hover",
                StyleSheet()
                    .WithBackgroundReference(BoxBackgroundTransparencyReference::Static(ColorRGB(56, 56, 56)))
            );

            control->OnMousePress = [action](Element& element)
            {
                if (action)
                {
                    action(element);
                }
            };

            return control;
        }

        void RefreshLayers()
        {
            m_List->ClearChildren();

            const auto& layers = m_Project->GetLayers();

            for (auto it = layers.rbegin(); it != layers.rend(); ++it)
            {
                m_List->AddChild(std::make_shared<LayerItem>(m_Project, *it));
            }
        }
    };
}