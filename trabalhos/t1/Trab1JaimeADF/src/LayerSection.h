// Summary:
// This file defines the `LayerSection` class, which represents a UI component for managing layers in a project.
// It provides functionality to display, create, delete, and reorder layers, as well as update the UI when layers are modified.

#pragma once

#include "BMP.h"
#include "Project.h"
#include "Box.h"
#include "Text.h"
#include "LayerItem.h"

namespace yap
{
    /**
     * @class LayerSection
     * @brief Represents a UI component for managing layers in a project.
     * 
     * The `LayerSection` class is a specialized `Box` that contains a header, a list of layers, 
     * and controls for managing layers. It interacts with a `Project` object to handle layer-related events.
     */
    class LayerSection : public Box
    {
    private:
        std::shared_ptr<Project> m_Project; ///< The project associated with this layer section.
        std::shared_ptr<Box> m_Header; ///< The header section of the UI.
        std::shared_ptr<Box> m_List; ///< The list section displaying the layers.
        std::shared_ptr<Box> m_Controls; ///< The controls section for managing layers.

    public:
        /**
         * @brief Constructs a `LayerSection` object.
         * @param project A shared pointer to the `Project` object associated with this layer section.
         */
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
        /**
         * @brief Creates the header section of the layer UI.
         * @return A shared pointer to the header `Box`.
         */
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

        /**
         * @brief Creates the list section of the layer UI.
         * @return A shared pointer to the list `Box`.
         */
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

        /**
         * @brief Creates the controls section of the layer UI.
         * @return A shared pointer to the controls `Box`.
         */
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

        /**
         * @brief Creates a control button with a bitmap and an associated action.
         * @param bitmap A shared pointer to the bitmap to display on the button.
         * @param action A function to execute when the button is pressed.
         * @return A shared pointer to the control `Box`.
         */
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

        /**
         * @brief Refreshes the list of layers displayed in the UI.
         * 
         * Clears the current list and repopulates it with the layers from the associated project.
         */
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