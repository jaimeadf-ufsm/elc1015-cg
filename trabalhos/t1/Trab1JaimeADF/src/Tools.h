// Summary:
// This file defines various tools for a graphical editing application, including MoveTool, TransformTool, RotateTool, BrushTool, BucketTool, and ColorPickerTool. 
// Each tool provides specific functionalities for interacting with and manipulating layers in a project. 
// The tools are implemented as classes inheriting from the base Tool class, and they include methods for creating overlays and options for user interaction.

#pragma once

#include "Project.h"
#include "ViewportSpace.h"
#include "ColorPalette.h"
#include "Brush.h"
#include "Screen.h"
#include "Text.h"
#include "Slider.h"
#include "LayerBoundary.h"

namespace yap
{
    // Base class for all tools.
    class Tool
    {
    protected:
        std::shared_ptr<Project> m_Project; // Reference to the project.
        std::shared_ptr<ViewportSpace> m_ViewportSpace; // Reference to the viewport space.

    public:
        // Constructor: Initializes the tool with a project and viewport space.
        Tool(const std::shared_ptr<Project>& project, const std::shared_ptr<ViewportSpace>& viewportSpace)
            : m_Project(project), m_ViewportSpace(viewportSpace)
        {
        }

        // Creates the overlay for the tool. Must be implemented by derived classes.
        virtual std::shared_ptr<Element> CreateOverlay() = 0;

        // Creates the options panel for the tool. Must be implemented by derived classes.
        virtual std::shared_ptr<Element> CreateOptions() = 0;
    };

    // Tool for moving layers.
    class MoveTool : public Tool
    {
    public:
        // Constructor: Initializes the MoveTool with a project and viewport space.
        MoveTool(const std::shared_ptr<Project>& project, const std::shared_ptr<ViewportSpace>& viewportSpace)
            : Tool(project, viewportSpace)
        {
        }

        // Creates the overlay for the MoveTool.
        std::shared_ptr<Element> CreateOverlay() override
        {
            return std::make_shared<MoveToolOverlay>(m_Project, m_ViewportSpace);
        }

        // Creates the options panel for the MoveTool (empty in this case).
        std::shared_ptr<Element> CreateOptions() override
        {
            return std::make_shared<Box>();
        }
    
    private:
        // Overlay class for handling move interactions.
        class MoveToolOverlay : public Box
        {
        private:
            std::shared_ptr<Project> m_Project; // Reference to the project.
            std::shared_ptr<ViewportSpace> m_ViewportSpace; // Reference to the viewport space.
            Vec2 m_CanvasOffset; // Offset for moving the layer.

        public:
            // Constructor: Initializes the overlay with project and viewport space.
            MoveToolOverlay(std::shared_ptr<Project> project, std::shared_ptr<ViewportSpace> viewportSpace)
                : m_Project(project), m_ViewportSpace(viewportSpace)
            {
                SetStyle(
                    StyleSheet()
                        .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
                );

                OnMousePress = [this](Element& element)
                {
                    const Mouse& mouse = element.GetScreen()->GetMouse();

                    Vec2 mouseCanvasPosition = m_ViewportSpace->ConvertScreenToCanvasCoordinates(mouse.Position);
                    mouseCanvasPosition.Floor();

                    std::shared_ptr<Layer> layer = m_Project->GetActiveLayer();

                    if (!layer)
                    {
                        return;
                    }

                    m_CanvasOffset = mouseCanvasPosition - layer->GetPosition();
                };

                OnMouseMove = [this](Element& element)
                {
                    const Mouse& mouse = element.GetScreen()->GetMouse();

                    if (element.IsPressed())
                    {
                        Vec2 mouseCanvasPosition = m_ViewportSpace->ConvertScreenToCanvasCoordinates(mouse.Position);
                        mouseCanvasPosition.Floor();

                        Vec2 newCanvasPosition = mouseCanvasPosition - m_CanvasOffset;

                        std::shared_ptr<Layer> layer = m_Project->GetActiveLayer();

                        if (!layer)
                        {
                            return;
                        }

                        layer->SetPosition(newCanvasPosition);
                    }
                };

                AddChild(std::make_shared<LayerBoundary>(m_Project, m_ViewportSpace));
            }
        };
    };

    // Tool for transforming layers (scaling and resizing).
    class TransformTool : public Tool
    {
    public:
        // Constructor: Initializes the TransformTool with a project and viewport space.
        TransformTool(const std::shared_ptr<Project>& project, const std::shared_ptr<ViewportSpace>& viewportSpace)
            : Tool(project, viewportSpace)
        {
        }

        // Creates the overlay for the TransformTool.
        std::shared_ptr<Element> CreateOverlay() override
        {
            return std::make_shared<TransformToolOverlay>(m_Project, m_ViewportSpace);
        }

        // Creates the options panel for the TransformTool (empty in this case).
        std::shared_ptr<Element> CreateOptions() override
        {
            return std::make_shared<Box>();
        }
    
    private:
        // Enum for anchor locations used during scaling.
        enum class AnchorLocation
        {
            TopLeft, TopMiddle, TopRight, MiddleRight, BottomRight, BottomMiddle, BottomLeft, MiddleLeft
        };

        // Overlay class for handling transform interactions.
        class TransformToolOverlay : public Box
        {
        private:
            std::shared_ptr<Project> m_Project; // Reference to the project.
            std::shared_ptr<ViewportSpace> m_ViewportSpace; // Reference to the viewport space.
            std::shared_ptr<Box> m_Boundary; // Boundary box for the layer.
            std::vector<std::shared_ptr<Box>> m_Anchors; // Anchors for resizing.
            bool m_Scaling = false; // Indicates if scaling is active.
            AnchorLocation m_ScalingLocation = AnchorLocation::TopLeft; // Current scaling anchor location.
            Vec2 m_TargetCanvasPosition; // Target position during scaling.
            Vec2 m_TargetCanvasSize; // Target size during scaling.
            Vec2 m_MouseStartCanvasPosition; // Mouse position at the start of scaling.

        public:
            // Constructor: Initializes the overlay with project and viewport space.
            TransformToolOverlay(std::shared_ptr<Project> project, std::shared_ptr<ViewportSpace> viewportSpace)
                : m_Project(project), m_ViewportSpace(viewportSpace)
            {
                OnAnimate = [this](Element& element)
                {
                    std::shared_ptr<Layer> layer = m_Project->GetActiveLayer();

                    SetStyle(
                        GetStyle()
                            .WithVisibility(layer != nullptr)
                    );

                    if (layer)
                    {
                        if (!m_Scaling)
                        {
                            m_TargetCanvasPosition = layer->GetPosition();
                            m_TargetCanvasSize = layer->GetSize();
                        }
                    }
                };

                InitBoundary();

                InitAnchor(AnchorLocation::TopLeft);
                InitAnchor(AnchorLocation::TopMiddle);
                InitAnchor(AnchorLocation::TopRight);
                InitAnchor(AnchorLocation::MiddleRight);
                InitAnchor(AnchorLocation::BottomRight);
                InitAnchor(AnchorLocation::BottomMiddle);
                InitAnchor(AnchorLocation::BottomLeft);
                InitAnchor(AnchorLocation::MiddleLeft);
            }

        private:
            // Initializes the boundary box.
            void InitBoundary()
            {
                m_Boundary = std::make_shared<Box>();

                m_Boundary->SetStyle(
                    StyleSheet()
                        .WithBorder(BoxBorder::Solid(ColorRGB(12, 140, 233), 2.0f))
                );

                m_Boundary->OnAnimate = [this](Element& element)
                {
                    Vec2 canvasStartPosition = m_TargetCanvasPosition;
                    Vec2 canvasEndPosition = m_TargetCanvasPosition + m_TargetCanvasSize;

                    Vec2 screenStartPosition = m_ViewportSpace->ConvertCanvasToScreenCoordinates(canvasStartPosition);
                    Vec2 screenEndPosition = m_ViewportSpace->ConvertCanvasToScreenCoordinates(canvasEndPosition);

                    screenStartPosition.Floor();
                    screenEndPosition.Floor();

                    screenStartPosition -= Vec2(2.0f);
                    screenEndPosition += Vec2(2.0f);

                    element.SetStyle(
                        element.GetStyle()
                            .WithSize(
                                AxisSizingRule::Fixed(screenEndPosition.X - screenStartPosition.X),
                                AxisSizingRule::Fixed(screenEndPosition.Y - screenStartPosition.Y)
                            )
                            .WithPosition(PositioningRule::Absolute(screenStartPosition))
                    );
                };

                AddChild(m_Boundary);
            }

            // Initializes an anchor for resizing.
            void InitAnchor(AnchorLocation location)
            {
                auto anchor = std::make_shared<Box>();

                anchor->SetStyle(
                    StyleSheet()
                        .WithSize(AxisSizingRule::Fixed(16), AxisSizingRule::Fixed(16))
                        .WithBackground(BoxBackground::Solid(ColorRGB(255, 255, 255)))
                        .WithBorder(BoxBorder::Solid(ColorRGB(12, 140, 233), 2.0f))
                );

                anchor->OnAnimate = [this, location](Element& element)
                {
                    Vec2 canvasPosition = ComputeAnchorCanvasPosition(location);
                    Vec2 screenPosition = m_ViewportSpace->ConvertCanvasToScreenCoordinates(canvasPosition);

                    screenPosition -= Vec2(9.0f);
                    screenPosition.Floor();

                    element.SetStyle(
                        element.GetStyle()
                            .WithPosition(PositioningRule::Absolute(screenPosition))
                    );
                };

                anchor->OnMouseMove = [this, location](Element& element)
                {
                    if (!m_Scaling || location != m_ScalingLocation)
                    {
                        return;
                    }

                    const Mouse& mouse = element.GetScreen()->GetMouse();

                    if (element.IsPressed())
                    {
                        Vec2 mouseCurrentCanvasPosition = m_ViewportSpace->ConvertScreenToCanvasCoordinates(mouse.Position);
                        Vec2 canvasDeltaPosition = mouseCurrentCanvasPosition - m_MouseStartCanvasPosition;

                        ScaleFromAnchor(location, canvasDeltaPosition);
                    }
                };

                anchor->OnMousePress = [this, location](Element& element)
                {
                    const Mouse& mouse = element.GetScreen()->GetMouse();

                    if (m_Scaling) 
                    {
                        return;
                    }

                    m_Scaling = true;
                    m_ScalingLocation = location;
                    m_MouseStartCanvasPosition = m_ViewportSpace->ConvertScreenToCanvasCoordinates(mouse.Position);
                };

                anchor->OnMouseRelease = [this, location](Element& element)
                {
                    if (!m_Scaling || location != m_ScalingLocation)
                    {
                        return;
                    }

                    std::shared_ptr<Layer> layer = m_Project->GetActiveLayer();

                    if (layer)
                    {
                        layer->SetPosition(m_TargetCanvasPosition);
                        layer->Scale(m_TargetCanvasSize, ScalingMethod::NearestNeighbor);
                    }

                    m_Scaling = false;
                };

                AddChild(anchor);
            }

            // Computes the canvas position of an anchor.
            Vec2 ComputeAnchorCanvasPosition(AnchorLocation location)
            {
                switch (location)
                {
                    case AnchorLocation::TopLeft:
                        return m_TargetCanvasPosition;
                    case AnchorLocation::TopMiddle:
                        return Vec2(
                            m_TargetCanvasPosition.X + std::floor(m_TargetCanvasSize.X / 2.0f),
                            m_TargetCanvasPosition.Y
                        );
                    case AnchorLocation::TopRight:
                        return Vec2(
                            m_TargetCanvasPosition.X + m_TargetCanvasSize.X,
                            m_TargetCanvasPosition.Y
                        );
                    case AnchorLocation::MiddleRight:
                        return Vec2(
                            m_TargetCanvasPosition.X + m_TargetCanvasSize.X,
                            m_TargetCanvasPosition.Y + std::floor(m_TargetCanvasSize.Y / 2.0f)
                        );
                    case AnchorLocation::BottomRight:
                        return Vec2(
                            m_TargetCanvasPosition.X + m_TargetCanvasSize.X,
                            m_TargetCanvasPosition.Y + m_TargetCanvasSize.Y
                        );
                    case AnchorLocation::BottomMiddle:
                        return Vec2(
                            m_TargetCanvasPosition.X + std::floor(m_TargetCanvasSize.X / 2.0f),
                            m_TargetCanvasPosition.Y + m_TargetCanvasSize.Y
                        );
                    case AnchorLocation::BottomLeft:
                        return Vec2(
                            m_TargetCanvasPosition.X,
                            m_TargetCanvasPosition.Y + m_TargetCanvasSize.Y
                        );
                    case AnchorLocation::MiddleLeft:
                        return Vec2(
                            m_TargetCanvasPosition.X,
                            m_TargetCanvasPosition.Y + std::floor(m_TargetCanvasSize.Y / 2.0f)
                        );
                }

                return Vec2(0.0f, 0.0f);
            }

            // Scales the layer from a specific anchor.
            void ScaleFromAnchor(AnchorLocation location, const Vec2& canvasDeltaPosition)
            {
                std::shared_ptr<Layer> layer = m_Project->GetActiveLayer();

                if (!layer)
                {
                    return;
                }

                const float minSize = 1.0f;

                Vec2 position = layer->GetPosition();
                Vec2 size = layer->GetSize();

                Vec2 newPosition = position;
                Vec2 newSize = size;

                switch (location)
                {
                    case AnchorLocation::TopLeft:
                        newPosition += canvasDeltaPosition;
                        newSize -= canvasDeltaPosition;
                        break;

                    case AnchorLocation::TopMiddle:
                        newPosition.Y += canvasDeltaPosition.Y;
                        newSize.Y -= canvasDeltaPosition.Y;
                        break;

                    case AnchorLocation::TopRight:
                        newPosition.Y += canvasDeltaPosition.Y;
                        newSize.X += canvasDeltaPosition.X;
                        newSize.Y -= canvasDeltaPosition.Y;
                        break;

                    case AnchorLocation::MiddleRight:
                        newSize.X += canvasDeltaPosition.X;
                        break;

                    case AnchorLocation::BottomRight:
                        newSize += canvasDeltaPosition;
                        break;

                    case AnchorLocation::BottomMiddle:
                        newSize.Y += canvasDeltaPosition.Y;
                        break;

                    case AnchorLocation::BottomLeft:
                        newPosition.X += canvasDeltaPosition.X;
                        newSize.X -= canvasDeltaPosition.X;
                        newSize.Y += canvasDeltaPosition.Y;
                        break;

                    case AnchorLocation::MiddleLeft:
                        newPosition.X += canvasDeltaPosition.X;
                        newSize.X -= canvasDeltaPosition.X;
                        break;
                }

                if (newSize.X < minSize)
                {
                    newSize.X = minSize;
                    if (location == AnchorLocation::TopLeft ||
                        location == AnchorLocation::MiddleLeft ||
                        location == AnchorLocation::BottomLeft)
                    {
                        newPosition.X = position.X + (size.X - minSize);
                    }
                }

                if (newSize.Y < minSize)
                {
                    newSize.Y = minSize;
                    if (location == AnchorLocation::TopLeft ||
                        location == AnchorLocation::TopMiddle ||
                        location == AnchorLocation::TopRight)
                    {
                        newPosition.Y = position.Y + (size.Y - minSize);
                    }
                }

                m_TargetCanvasPosition = newPosition;
                m_TargetCanvasSize = newSize;
            }
        };
    };

    // Tool for rotating layers.
    class RotateTool : public Tool
    {
    public:
        // Constructor: Initializes the RotateTool with a project and viewport space.
        RotateTool(const std::shared_ptr<Project>& project, const std::shared_ptr<ViewportSpace>& viewportSpace)
            : Tool(project, viewportSpace)
        {
        }

        // Creates the overlay for the RotateTool.
        std::shared_ptr<Element> CreateOverlay() override
        {
            return std::make_shared<RotateToolOverlay>(m_Project, m_ViewportSpace);
        }

        // Creates the options panel for the RotateTool (empty in this case).
        std::shared_ptr<Element> CreateOptions() override
        {
            return std::make_shared<Box>();
        }
    
    private:
        // Overlay class for handling rotation interactions.
        class RotateToolOverlay : public Element
        {
        private:
            std::shared_ptr<Project> m_Project; // Reference to the project.
            std::shared_ptr<ViewportSpace> m_ViewportSpace; // Reference to the viewport space.
            std::vector<Vec2> m_CanvasCorners; // Corners of the layer in canvas space.
            Vec2 m_CanvasTopLeft; // Top-left corner of the layer.
            Vec2 m_CanvasBottomRight; // Bottom-right corner of the layer.
            std::vector<Vec2> m_ScreenCorners; // Corners of the layer in screen space.
            Vec2 m_CanvasPivot; // Pivot point for rotation.
            float m_CanvasRotation = 0.0f; // Current rotation angle.
            Vec2 m_LastMousePosition; // Last mouse position.

        public:
            // Constructor: Initializes the overlay with project and viewport space.
            RotateToolOverlay(std::shared_ptr<Project> project, std::shared_ptr<ViewportSpace> viewportSpace)
                : m_Project(project), m_ViewportSpace(viewportSpace), m_CanvasCorners(4), m_ScreenCorners(4)
            {
                SetStyle(
                    StyleSheet()
                        .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
                );

                OnAnimate = [this](Element& element)
                {
                    std::shared_ptr<Layer> activeLayer = m_Project->GetActiveLayer();

                    if (activeLayer)
                    {
                        m_CanvasPivot = activeLayer->GetPosition() + activeLayer->GetSize() / 2.0f;
                        RefreshBounds();
                    }
                };

                OnMousePress = [this](Element& element)
                {
                    const Mouse& mouse = element.GetScreen()->GetMouse();

                    std::shared_ptr<Layer> activeLayer = m_Project->GetActiveLayer();

                    if (activeLayer)
                    {
                        m_CanvasPivot = activeLayer->GetPosition() + activeLayer->GetSize() / 2.0f;
                        m_CanvasRotation = 0.0f;
                        m_LastMousePosition = mouse.Position;
                    }
                };

                OnMouseMove = [this](Element& element)
                {
                    const Mouse& mouse = element.GetScreen()->GetMouse();
                    const Keyboard& keyboard = element.GetScreen()->GetKeyboard();

                    if (element.IsPressed())
                    {
                        std::shared_ptr<Layer> activeLayer = m_Project->GetActiveLayer();

                        if (activeLayer)
                        {
                            Vec2 currentMousePosition = mouse.Position;

                            Vec2 startCanvasPosition = m_ViewportSpace->ConvertScreenToCanvasCoordinates(m_LastMousePosition) - m_CanvasPivot;
                            Vec2 endCanvasPosition = m_ViewportSpace->ConvertScreenToCanvasCoordinates(currentMousePosition) - m_CanvasPivot;

                            startCanvasPosition.Normalize();
                            endCanvasPosition.Normalize();

                            m_CanvasRotation = endCanvasPosition.Angle() - startCanvasPosition.Angle();

                            if (keyboard.IsModifierEnabled(KeyboardModifier::Shift))
                            {
                                m_CanvasRotation = std::round(m_CanvasRotation / 15.0f) * 15.0f;
                            }
                        }
                    }
                };

                OnMouseRelease = [this](Element& element)
                {
                    std::shared_ptr<Layer> activeLayer = m_Project->GetActiveLayer();

                    if (activeLayer)
                    {
                        activeLayer->Rotate(m_CanvasRotation, m_CanvasPivot);
                        m_CanvasRotation = 0.0f;
                    }
                };
            }

            // Draws the rotation overlay.
            void Draw(RenderingContext& context) override
            {
                std::shared_ptr<Layer> activeLayer = m_Project->GetActiveLayer();

                if (!activeLayer)
                {
                    return;
                }

                Vec2 pivotScreenPosition = m_ViewportSpace->ConvertCanvasToScreenCoordinates(m_CanvasPivot);
                pivotScreenPosition.Floor();

                Vec2 horizontalDirection(1.0f, 0.0f);
                Vec2 verticalDirection(0.0f, 1.0f);

                horizontalDirection.Rotate(m_CanvasRotation);
                verticalDirection.Rotate(m_CanvasRotation);

                Vec2 horizontalStart = pivotScreenPosition - horizontalDirection * 32.0f;
                Vec2 horizontalEnd = pivotScreenPosition + horizontalDirection * 32.0f;

                Vec2 verticalStart = pivotScreenPosition - verticalDirection * 32.0f;
                Vec2 verticalEnd = pivotScreenPosition + verticalDirection * 32.0f;

                Vec2 screenTopLeft = m_ViewportSpace->ConvertCanvasToScreenCoordinates(m_CanvasTopLeft) - Vec2(2.0f);
                Vec2 screenBottomRight = m_ViewportSpace->ConvertCanvasToScreenCoordinates(m_CanvasBottomRight) + Vec2(2.0f);

                m_ScreenCorners[0] = m_ViewportSpace->ConvertCanvasToScreenCoordinates(m_CanvasCorners[0]) - horizontalDirection - verticalDirection;
                m_ScreenCorners[1] = m_ViewportSpace->ConvertCanvasToScreenCoordinates(m_CanvasCorners[1]) + horizontalDirection - verticalDirection;
                m_ScreenCorners[2] = m_ViewportSpace->ConvertCanvasToScreenCoordinates(m_CanvasCorners[2]) - horizontalDirection + verticalDirection;
                m_ScreenCorners[3] = m_ViewportSpace->ConvertCanvasToScreenCoordinates(m_CanvasCorners[3]) + horizontalDirection + verticalDirection;

                context.Color(ColorRGB(200, 64, 33));

                context.StrokeRectangle(
                    screenTopLeft,
                    screenBottomRight - screenTopLeft,
                    2.0f
                );

                context.Color(ColorRGB(12, 140, 233));
                context.Line(horizontalStart, horizontalEnd, 2.0f);
                context.Line(verticalStart, verticalEnd, 2.0f);

                context.Line(m_ScreenCorners[0], m_ScreenCorners[1], 2.0f);
                context.Line(m_ScreenCorners[1], m_ScreenCorners[3], 2.0f);
                context.Line(m_ScreenCorners[3], m_ScreenCorners[2], 2.0f);
                context.Line(m_ScreenCorners[2], m_ScreenCorners[0], 2.0f);
            }

        private:
            // Refreshes the bounds of the layer.
            void RefreshBounds()
            {
                std::shared_ptr<Layer> activeLayer = m_Project->GetActiveLayer();

                if (activeLayer)
                {
                    Vec2 layerSize = activeLayer->GetSize();
                    Vec2 layerPosition = activeLayer->GetPosition();

                    m_CanvasCorners.clear();

                    m_CanvasCorners.push_back(layerPosition);
                    m_CanvasCorners.push_back(layerPosition + Vec2(layerSize.X, 0.0f));
                    m_CanvasCorners.push_back(layerPosition + Vec2(0.0f, layerSize.Y));
                    m_CanvasCorners.push_back(layerPosition + layerSize);

                    for (auto& corner : m_CanvasCorners)
                    {
                        corner.Rotate(m_CanvasRotation, m_CanvasPivot);
                        corner.Floor();
                    }

                    m_CanvasTopLeft = m_CanvasCorners[0];
                    m_CanvasBottomRight = m_CanvasCorners[0];

                    for (const auto& corner : m_CanvasCorners)
                    {
                        m_CanvasTopLeft.X = std::min(m_CanvasTopLeft.X, corner.X);
                        m_CanvasTopLeft.Y = std::min(m_CanvasTopLeft.Y, corner.Y);
                        m_CanvasBottomRight.X = std::max(m_CanvasBottomRight.X, corner.X);
                        m_CanvasBottomRight.Y = std::max(m_CanvasBottomRight.Y, corner.Y);
                    }

                    m_CanvasTopLeft.Floor();
                    m_CanvasBottomRight.Floor();
                }
            }
        };
    };

    // Tool for drawing with a brush.
    class BrushTool : public Tool
    {
    private:
        std::shared_ptr<Brush> m_Brush; // Reference to the brush.

    public:
        // Constructor: Initializes the BrushTool with a project, viewport space, and brush.
        BrushTool(const std::shared_ptr<Project>& project, const std::shared_ptr<ViewportSpace>& viewportSpace, const std::shared_ptr<Brush>& brush)
            : Tool(project, viewportSpace), m_Brush(brush)
        {
        }

        // Creates the overlay for the BrushTool.
        std::shared_ptr<Element> CreateOverlay() override
        {
            return std::make_shared<BrushToolOverlay>(m_Project, m_ViewportSpace, m_Brush);
        }

        // Creates the options panel for the BrushTool.
        std::shared_ptr<Element> CreateOptions() override
        {
            return std::make_shared<BrushToolOptions>(m_Brush);
        }
   
    private:
        // Overlay class for handling brush interactions.
        class BrushToolOverlay : public Box
        {
        private:
            std::shared_ptr<Project> m_Project; // Reference to the project.
            std::shared_ptr<ViewportSpace> m_ViewportSpace; // Reference to the viewport space.
            std::shared_ptr<Brush> m_Brush; // Reference to the brush.
            Vec2 m_LastMousePosition; // Last mouse position.

        public:
            // Constructor: Initializes the overlay with project, viewport space, and brush.
            BrushToolOverlay(std::shared_ptr<Project> project, std::shared_ptr<ViewportSpace> viewportSpace, std::shared_ptr<Brush> brush)
                : m_Project(project), m_ViewportSpace(viewportSpace), m_Brush(brush)
            {
                SetStyle(
                    StyleSheet()
                        .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
                );

                OnMousePress = [this](Element& element)
                {
                    const Mouse& mouse = element.GetScreen()->GetMouse();

                    std::shared_ptr<Layer> activeLayer = m_Project->GetActiveLayer();

                    if (activeLayer)
                    {
                        m_Brush->Apply(
                            activeLayer,
                            m_ViewportSpace->ConvertScreenToCanvasCoordinates(mouse.Position)
                        );
                    }

                    m_LastMousePosition = mouse.Position;
                };

                OnMouseMove = [this](Element& element)
                {
                    const Mouse& mouse = element.GetScreen()->GetMouse();

                    if (element.IsPressed())
                    {
                        Vec2 currentMousePosition = mouse.Position;

                        Vec2 startCanvasPosition = m_ViewportSpace->ConvertScreenToCanvasCoordinates(m_LastMousePosition);
                        Vec2 endCanvasPosition = m_ViewportSpace->ConvertScreenToCanvasCoordinates(currentMousePosition);

                        std::shared_ptr<Layer> activeLayer = m_Project->GetActiveLayer();

                        if (activeLayer)
                        {
                            m_Brush->Stroke(activeLayer, startCanvasPosition, endCanvasPosition);
                        }

                        m_LastMousePosition = currentMousePosition;
                    }
                };

                AddChild(std::make_shared<LayerBoundary>(m_Project, m_ViewportSpace));
            }
        };

        // Options panel for configuring the brush.
        class BrushToolOptions : public Box
        {
        public:
            // Constructor: Initializes the options panel with a brush.
            BrushToolOptions(std::shared_ptr<Brush> brush)
            {
                auto sizeLabel = std::make_shared<Text>();
                auto sizeSlider = std::make_shared<Slider>();
                auto sizeValue = std::make_shared<Text>();

                auto shapeLabel = std::make_shared<Text>();
                auto shapeButtons = std::make_shared<Box>();

                sizeLabel->Content = "Tamanho:";
                sizeValue->Content = std::to_string(static_cast<int>(brush->GetSize())) + " px";
                
                sizeSlider->MinValue = 1.0f;
                sizeSlider->MaxValue = 100.0f;
                sizeSlider->Step = 1.0f;

                sizeSlider->SetStyle(
                    sizeSlider->GetStyle()
                        .WithSize(AxisSizingRule::Fixed(127), AxisSizingRule::Fixed(16))
                );

                sizeSlider->OnChange = [this, brush, sizeValue](Slider& slider, float value)
                {
                    int size = static_cast<int>(value);

                    brush->SetSize(size);
                    sizeValue->Content = std::to_string(size) + " px";
                };

                std::shared_ptr<PencilBrush> pencilBrush = std::dynamic_pointer_cast<PencilBrush>(brush);

                if (pencilBrush)
                {
                    shapeLabel->Content = "Forma:";

                    shapeButtons->SetStyle(
                        StyleSheet()
                            .WithSize(AxisSizingRule::Fit(), AxisSizingRule::Fixed(24))
                            .WithAlignment(BoxAxisAlignment::Start, BoxAxisAlignment::Center)
                            .WithGap(4)
                    );

                    shapeButtons->AddChild(
                        CreateShapeButton(
                            std::make_shared<Bitmap>(BMP::Load("Trab1JaimeADF/assets/circle-24x24.bmp")),
                            pencilBrush,
                            PencilShape::Circle
                        )
                    );

                    shapeButtons->AddChild(
                        CreateShapeButton(
                            std::make_shared<Bitmap>(BMP::Load("Trab1JaimeADF/assets/square-24x24.bmp")),
                            pencilBrush,
                            PencilShape::Square
                        )
                    );

                    shapeButtons->AddChild(
                        CreateShapeButton(
                            std::make_shared<Bitmap>(BMP::Load("Trab1JaimeADF/assets/triangle-24x24.bmp")),
                            pencilBrush,
                            PencilShape::Triangle
                        )
                    );
                }

                SetStyle(
                    StyleSheet()
                        .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
                        .WithAlignment(BoxAxisAlignment::Start, BoxAxisAlignment::Center)
                        .WithForeground(ColorRGB(255, 255, 255))
                        .WithGap(16)
                );

                AddChild(sizeLabel);
                AddChild(sizeSlider);
                AddChild(sizeValue);
                AddChild(shapeLabel);
                AddChild(shapeButtons);
            }
        
        private:
            // Creates a button for selecting a brush shape.
            std::shared_ptr<Box> CreateShapeButton(const std::shared_ptr<const Bitmap>& icon, const std::shared_ptr<PencilBrush> brush, PencilShape shape)
            {
                auto button = std::make_shared<Box>();

                button->SetStyle(
                    StyleSheet()
                        .WithSize(AxisSizingRule::Fixed(24), AxisSizingRule::Fixed(24))
                        .WithBackground(BoxBackground::Image(icon))
                        .WithBackgroundReference(BoxBackgroundTransparencyReference::Static(ColorRGB(44, 44, 44)))
                );

                button->SetStyle(
                    ":hover",
                    StyleSheet()
                        .WithBackgroundReference(BoxBackgroundTransparencyReference::Static(ColorRGB(56, 56, 56)))
                );

                button->SetStyle(
                    "selected",
                    StyleSheet()
                        .WithBackgroundReference(BoxBackgroundTransparencyReference::Static(ColorRGB(74, 80, 124)))
                );

                button->OnAnimate = [this, brush, shape](Element& element)
                {
                    element.ToggleTrait("selected", brush->GetShape() == shape);
                };

                button->OnMousePress = [this, brush, shape](Element& element)
                {
                    brush->SetShape(shape);
                };

                return button;
            }
        };
    };

    // Tool for filling areas with a color.
    class BucketTool : public Tool
    {
    private:
        std::shared_ptr<ColorPalette> m_ColorPalette; // Reference to the color palette.


    public:
        // Constructor: Initializes the BucketTool with a project, viewport space, and color palette.
        BucketTool(const std::shared_ptr<Project>& project, const std::shared_ptr<ViewportSpace>& viewportSpace, const std::shared_ptr<ColorPalette>& colorPalette)
            : Tool(project, viewportSpace), m_ColorPalette(colorPalette)
        {
        }

        // Creates the overlay for the BucketTool.
        std::shared_ptr<Element> CreateOverlay() override
        {
            return std::make_shared<BucketToolOverlay>(m_Project, m_ViewportSpace, m_ColorPalette);
        }

        // Creates the options panel for the BucketTool (empty in this case).
        std::shared_ptr<Element> CreateOptions() override
        {
            return std::make_shared<Box>();
        }

    private:
        // Overlay class for handling bucket fill interactions.
        class BucketToolOverlay : public Box
        {
        private:
            std::shared_ptr<Project> m_Project; // Reference to the project.
            std::shared_ptr<ViewportSpace> m_ViewportSpace; // Reference to the viewport space.
            std::shared_ptr<ColorPalette> m_ColorPalette; // Reference to the color palette.

        public:
            // Constructor: Initializes the overlay with project, viewport space, and color palette.
            BucketToolOverlay(std::shared_ptr<Project> project, std::shared_ptr<ViewportSpace> viewportSpace, std::shared_ptr<ColorPalette> colorPalette)
                : m_Project(project), m_ViewportSpace(viewportSpace), m_ColorPalette(colorPalette)
            {
                SetStyle(
                    StyleSheet()
                        .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
                );

                OnMousePress = [this](Element& element)
                {
                    const Mouse& mouse = element.GetScreen()->GetMouse();

                    std::shared_ptr<Layer> activeLayer = m_Project->GetActiveLayer();

                    if (activeLayer)
                    {
                        Vec2 canvasPosition = m_ViewportSpace->ConvertScreenToCanvasCoordinates(mouse.Position);
                        canvasPosition.Floor();

                        ColorRGB fillColor = m_ColorPalette->GetGlobalColor();

                        activeLayer->Fill(canvasPosition, fillColor);
                    }
                };

                AddChild(std::make_shared<LayerBoundary>(m_Project, m_ViewportSpace));
            }
        };
    };
    
    // Tool for picking colors from the canvas.
    class ColorPickerTool : public Tool
    {
    private:
        std::shared_ptr<ColorPalette> m_ColorPalette; // Reference to the color palette.

    public:
        // Constructor: Initializes the ColorPickerTool with a project, viewport space, and color palette.
        ColorPickerTool(const std::shared_ptr<Project>& project, const std::shared_ptr<ViewportSpace>& viewportSpace, const std::shared_ptr<ColorPalette>& colorPalette)
            : Tool(project, viewportSpace), m_ColorPalette(colorPalette)
        {
        }

        // Creates the overlay for the ColorPickerTool.
        std::shared_ptr<Element> CreateOverlay() override
        {
            return std::make_shared<ColorPickerToolOverlay>(m_Project, m_ViewportSpace, m_ColorPalette);
        }

        // Creates the options panel for the ColorPickerTool (empty in this case).
        std::shared_ptr<Element> CreateOptions() override
        {
            return std::make_shared<Box>();
        }
    
    private:
        // Overlay class for handling color picking interactions.
        class ColorPickerToolOverlay : public Box
        {
        private:
            std::shared_ptr<Project> m_Project; // Reference to the project.
            std::shared_ptr<ViewportSpace> m_ViewportSpace; // Reference to the viewport space.
            std::shared_ptr<ColorPalette> m_ColorPalette; // Reference to the color palette.

        public:
            // Constructor: Initializes the overlay with project, viewport space, and color palette.
            ColorPickerToolOverlay(std::shared_ptr<Project> project, std::shared_ptr<ViewportSpace> viewportSpace, std::shared_ptr<ColorPalette> colorPalette)
                : m_Project(project), m_ViewportSpace(viewportSpace), m_ColorPalette(colorPalette)
            {
                SetStyle(
                    StyleSheet()
                        .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
                );

                OnMousePress = [this](Element& element)
                {
                    const Mouse& mouse = element.GetScreen()->GetMouse();

                    std::shared_ptr<const Bitmap> canvas = m_Project->GetCanvas();

                    Vec2 canvasPosition = m_ViewportSpace->ConvertScreenToCanvasCoordinates(mouse.Position);
                    canvasPosition.Floor();

                    if (canvasPosition.X < 0 || canvasPosition.Y < 0 ||
                        canvasPosition.X >= canvas->GetWidth() || canvasPosition.Y >= canvas->GetHeight())
                    {
                        return;
                    }

                    ColorRGBA color = canvas->GetPixel(canvasPosition.X, canvasPosition.Y);

                    m_ColorPalette->SetGlobalColor(color);
                };
            }
        };
    };
}