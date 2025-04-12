#pragma once

#include "Project.h"
#include "ViewportSpace.h"

#include "Box.h"

/**
 * @file LayerBoundary.h
 * @brief Defines the UI component responsible for displaying the boundary of a
 * layer in the viewport.
 */

namespace yap
{
    /**
     * @class LayerBoundary
     * @brief Represents a visual boundary for a layer in the application.
     * 
     * The `LayerBoundary` class inherits from `Box` and is responsible for dynamically
     * updating its size, position, and visibility based on the active layer's properties
     * and the viewport space. It uses a lambda function to handle animations and updates.
     */
    class LayerBoundary : public Box
    {
    private:
        std::shared_ptr<Project> m_Project;
        std::shared_ptr<ViewportSpace> m_ViewportSpace;

    public:
        LayerBoundary(std::shared_ptr<Project> project, std::shared_ptr<ViewportSpace> viewportSpace)
            : m_Project(project), m_ViewportSpace(viewportSpace)
        {
            SetStyle(
                StyleSheet()
                    .WithBorder(BoxBorder::Solid(ColorRGB(12, 140, 233), 2.0f))
            );

            OnAnimate = [this](Element& element)
            {
                std::shared_ptr<Layer> layer = m_Project->GetActiveLayer();

                if (layer)
                {
                    Vec2 canvasStartPosition = layer->GetPosition();
                    Vec2 canvasEndPosition = layer->GetPosition() + layer->GetSize();

                    Vec2 screenStartPosition = m_ViewportSpace->ConvertCanvasToScreenCoordinates(canvasStartPosition);
                    Vec2 screenEndPosition = m_ViewportSpace->ConvertCanvasToScreenCoordinates(canvasEndPosition);

                    screenStartPosition.Floor();
                    screenEndPosition.Floor();

                    screenStartPosition -= Vec2(2.0f);
                    screenEndPosition += Vec2(2.0f);

                    SetStyle(
                        GetStyle()
                            .WithVisibility(true)
                            .WithSize(
                                AxisSizingRule::Fixed(screenEndPosition.X - screenStartPosition.X),
                                AxisSizingRule::Fixed(screenEndPosition.Y - screenStartPosition.Y)
                            )
                            .WithPosition(PositioningRule::Absolute(screenStartPosition))
                    );
                }
                else
                {
                    SetStyle(
                        GetStyle()
                            .WithVisibility(false)
                    );
                }
            };
        }
    };
};