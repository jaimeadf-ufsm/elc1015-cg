#pragma once

// Summary:
// This file defines the `LayerBoundary` class, which is responsible for managing
// the visual boundary of a layer in a graphical application. It dynamically adjusts
// its size and position based on the active layer's properties and viewport space.

#include "Project.h"
#include "ViewportSpace.h"
#include "Box.h"

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
        /// A shared pointer to the `Project` instance, used to access the active layer.
        std::shared_ptr<Project> m_Project;

        /// A shared pointer to the `ViewportSpace` instance, used for coordinate conversions.
        std::shared_ptr<ViewportSpace> m_ViewportSpace;

    public:
        /**
         * @brief Constructs a `LayerBoundary` object.
         * 
         * @param project A shared pointer to the `Project` instance.
         * @param viewportSpace A shared pointer to the `ViewportSpace` instance.
         * 
         * Initializes the `LayerBoundary` with a default border style and sets up
         * an animation handler to update its properties dynamically.
         */
        LayerBoundary(std::shared_ptr<Project> project, std::shared_ptr<ViewportSpace> viewportSpace)
            : m_Project(project), m_ViewportSpace(viewportSpace)
        {
            // Set the initial style of the boundary with a solid border.
            SetStyle(
                StyleSheet()
                    .WithBorder(BoxBorder::Solid(ColorRGB(12, 140, 233), 2.0f))
            );

            // Define the animation handler to update the boundary dynamically.
            OnAnimate = [this](Element& element)
            {
                // Retrieve the active layer from the project.
                std::shared_ptr<Layer> layer = m_Project->GetActiveLayer();

                if (layer)
                {
                    // Calculate the layer's start and end positions in canvas coordinates.
                    Vec2 canvasStartPosition = layer->GetPosition();
                    Vec2 canvasEndPosition = layer->GetPosition() + layer->GetSize();

                    // Convert canvas coordinates to screen coordinates.
                    Vec2 screenStartPosition = m_ViewportSpace->ConvertCanvasToScreenCoordinates(canvasStartPosition);
                    Vec2 screenEndPosition = m_ViewportSpace->ConvertCanvasToScreenCoordinates(canvasEndPosition);

                    // Adjust the positions to align with pixel boundaries.
                    screenStartPosition.Floor();
                    screenEndPosition.Floor();

                    // Add padding to the boundary.
                    screenStartPosition -= Vec2(2.0f);
                    screenEndPosition += Vec2(2.0f);

                    // Update the style with the new size, position, and visibility.
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
                    // Hide the boundary if no active layer is present.
                    SetStyle(
                        GetStyle()
                            .WithVisibility(false)
                    );
                }
            };
        }
    };
};