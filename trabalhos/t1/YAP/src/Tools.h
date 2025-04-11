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
    class Tool
    {
    protected:
        std::shared_ptr<Project> m_Project;
        std::shared_ptr<ViewportSpace> m_ViewportSpace;

    public:
        Tool(const std::shared_ptr<Project>& project, const std::shared_ptr<ViewportSpace>& viewportSpace)
            : m_Project(project), m_ViewportSpace(viewportSpace)
        {
        }

        virtual std::shared_ptr<Element> CreateOverlay() = 0;
        virtual std::shared_ptr<Element> CreateOptions() = 0;
    };

    class MoveTool : public Tool
    {
    public:
        MoveTool(const std::shared_ptr<Project>& project, const std::shared_ptr<ViewportSpace>& viewportSpace)
            : Tool(project, viewportSpace)
        {
        }

        std::shared_ptr<Element> CreateOverlay() override
        {
            return std::make_shared<MoveToolOverlay>(m_Project, m_ViewportSpace);
        }

        std::shared_ptr<Element> CreateOptions() override
        {
            return std::make_shared<Box>();
        }
    
    private:
        class MoveToolOverlay : public Box
        {
        private:
            std::shared_ptr<Project> m_Project;
            std::shared_ptr<ViewportSpace> m_ViewportSpace;

            Vec2 m_CanvasOffset;

        public:
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

    class TransformTool : public Tool
    {
    public:
        TransformTool(const std::shared_ptr<Project>& project, const std::shared_ptr<ViewportSpace>& viewportSpace)
            : Tool(project, viewportSpace)
        {
        }

        std::shared_ptr<Element> CreateOverlay() override
        {
            return std::make_shared<TransformToolOverlay>(m_Project, m_ViewportSpace);
        }

        std::shared_ptr<Element> CreateOptions() override
        {
            return std::make_shared<Box>();
        }
    
    private:
        enum class AnchorLocation
        {
            TopLeft,
            TopMiddle,
            TopRight,
            MiddleRight,
            BottomRight,
            BottomMiddle,
            BottomLeft,
            MiddleLeft
        };

        class TransformToolOverlay : public Box
        {
        private:
            std::shared_ptr<Project> m_Project;
            std::shared_ptr<ViewportSpace> m_ViewportSpace;

            std::shared_ptr<Box> m_Boundary;
            std::vector<std::shared_ptr<Box>> m_Anchors;

            bool m_Scaling = false;
            AnchorLocation m_ScalingLocation = AnchorLocation::TopLeft;

            Vec2 m_TargetCanvasPosition;
            Vec2 m_TargetCanvasSize;

            Vec2 m_MouseStartCanvasPosition;

        public:
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

    class RotateTool : public Tool
    {
    public:
        RotateTool(const std::shared_ptr<Project>& project, const std::shared_ptr<ViewportSpace>& viewportSpace)
            : Tool(project, viewportSpace)
        {
        }

        std::shared_ptr<Element> CreateOverlay() override
        {
            return std::make_shared<RotateToolOverlay>(m_Project, m_ViewportSpace);
        }

        std::shared_ptr<Element> CreateOptions() override
        {
            return std::make_shared<Box>();
        }
    
    private:
        class RotateToolOverlay : public Element
        {
        private:
            std::shared_ptr<Project> m_Project;
            std::shared_ptr<ViewportSpace> m_ViewportSpace;

            std::vector<Vec2> m_CanvasCorners;

            Vec2 m_CanvasTopLeft;
            Vec2 m_CanvasBottomRight;

            std::vector<Vec2> m_ScreenCorners;

            Vec2 m_CanvasPivot;
            float m_CanvasRotation = 0.0f;

            Vec2 m_LastMousePosition;

        public:
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

    class BrushTool : public Tool
    {
    private:
        std::shared_ptr<Brush> m_Brush;

    public:
        BrushTool(const std::shared_ptr<Project>& project, const std::shared_ptr<ViewportSpace>& viewportSpace, const std::shared_ptr<Brush>& brush)
            : Tool(project, viewportSpace), m_Brush(brush)
        {
        }

        std::shared_ptr<Element> CreateOverlay() override
        {
            return std::make_shared<BrushToolOverlay>(m_Project, m_ViewportSpace, m_Brush);
        }

        std::shared_ptr<Element> CreateOptions() override
        {
            return std::make_shared<BrushToolOptions>(m_Brush);
        }
   
    private:
        class BrushToolOverlay : public Box
        {
        private:
            std::shared_ptr<Project> m_Project;
            std::shared_ptr<ViewportSpace> m_ViewportSpace;

            std::shared_ptr<Brush> m_Brush;

            Vec2 m_LastMousePosition;

        public:
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

        class BrushToolOptions : public Box
        {
        public:
            BrushToolOptions(std::shared_ptr<Brush> brush)
            {
                auto sizeLabel = std::make_shared<Text>();
                auto sizeSlider = std::make_shared<Slider>();
                auto sizeValue = std::make_shared<Text>();

                sizeLabel->Content = "Brush Size:";
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
            }
        };
    };

    class BucketTool : public Tool
    {
    private:
        std::shared_ptr<ColorPalette> m_ColorPalette;


    public:
        BucketTool(const std::shared_ptr<Project>& project, const std::shared_ptr<ViewportSpace>& viewportSpace, const std::shared_ptr<ColorPalette>& colorPalette)
            : Tool(project, viewportSpace), m_ColorPalette(colorPalette)
        {
        }

        std::shared_ptr<Element> CreateOverlay() override
        {
            return std::make_shared<BucketToolOverlay>(m_Project, m_ViewportSpace, m_ColorPalette);
        }

        std::shared_ptr<Element> CreateOptions() override
        {
            return std::make_shared<Box>();
        }

    private:
        class BucketToolOverlay : public Box
        {
        private:
            std::shared_ptr<Project> m_Project;
            std::shared_ptr<ViewportSpace> m_ViewportSpace;

            std::shared_ptr<ColorPalette> m_ColorPalette;

        public:
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
    
    class ColorPickerTool : public Tool
    {
    private:
        std::shared_ptr<ColorPalette> m_ColorPalette;

    public:
        ColorPickerTool(const std::shared_ptr<Project>& project, const std::shared_ptr<ViewportSpace>& viewportSpace, const std::shared_ptr<ColorPalette>& colorPalette)
            : Tool(project, viewportSpace), m_ColorPalette(colorPalette)
        {
        }

        std::shared_ptr<Element> CreateOverlay() override
        {
            return std::make_shared<ColorPickerToolOverlay>(m_Project, m_ViewportSpace, m_ColorPalette);
        }

        std::shared_ptr<Element> CreateOptions() override
        {
            return std::make_shared<Box>();
        }
    
    private:
        class ColorPickerToolOverlay : public Box
        {
        private:
            std::shared_ptr<Project> m_Project;
            std::shared_ptr<ViewportSpace> m_ViewportSpace;
            std::shared_ptr<ColorPalette> m_ColorPalette;

        public:
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