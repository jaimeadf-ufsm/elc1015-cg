#pragma once

#include "Project.h"

#include "ColorSection.h"
#include "LayerSection.h"
#include "Tool.h"
#include "Slider.h"

namespace yap
{
    class Workspace : public Box
    {
    private:
        std::shared_ptr<Project> m_Project;
        std::shared_ptr<ColorPalette> m_ColorPalette;
        std::shared_ptr<ViewportSpace> m_ViewportSpace;

        std::shared_ptr<Box> m_Viewport;
        std::shared_ptr<Box> m_ViewportPreview;
        std::shared_ptr<Box> m_ViewportOverlay;

        std::shared_ptr<Box> m_ToolBar;
        std::shared_ptr<Box> m_ToolBarTools;
        std::shared_ptr<Box> m_ToolBarDivider;

        std::shared_ptr<Box> m_SideBar;
    
    public:
        Workspace()
        {
            m_Project = std::make_shared<Project>(300, 300);
            m_ColorPalette = std::make_shared<ColorPalette>(ColorRGBA(255, 0, 0, 255));

            InitViewport();

            m_ViewportSpace = std::make_shared<ViewportSpace>(m_Project, m_ViewportPreview);

            InitToolBar();
            InitSideBar();

            PencilBrush brush(m_ColorPalette);

            BrushTool tool(m_Project, m_ViewportSpace, std::make_shared<PencilBrush>(m_ColorPalette));

            InitToolButton(
                std::make_shared<Bitmap>(BMP::Load("YAP/assets/move.bmp")),
                std::make_shared<MoveTool>(m_Project, m_ViewportSpace)
            );

            InitToolButton(
                std::make_shared<Bitmap>(BMP::Load("YAP/assets/transform.bmp")),
                std::make_shared<TransformTool>(m_Project, m_ViewportSpace)
            );

            InitToolButton(
                std::make_shared<Bitmap>(BMP::Load("YAP/assets/brush.bmp")),
                std::make_shared<BrushTool>(m_Project, m_ViewportSpace, std::make_shared<PencilBrush>(m_ColorPalette))
            );

            InitToolButton(
                std::make_shared<Bitmap>(BMP::Load("YAP/assets/eraser.bmp")),
                std::make_shared<BrushTool>(m_Project, m_ViewportSpace, std::make_shared<PencilBrush>(std::make_shared<ColorPalette>(ColorRGBA(0, 0, 0, 0))))
            );

            AddChild(m_ToolBar);
            AddChild(m_Viewport);
            AddChild(m_SideBar);

            for (int i = 0; i < 10; ++i)
            {
                m_Project->CreateLayer();
            }

            AddChild(std::make_shared<Slider>());
        }

        void Animate() override
        {
            Box::Animate();

            std::shared_ptr<const Bitmap> projection = m_Project->Render();

            m_ViewportPreview->SetStyle(
                m_ViewportPreview->GetStyle()
                    .WithSize(AxisSizingRule::Fixed(projection->GetWidth()), AxisSizingRule::Fixed(projection->GetHeight()))
                    .WithBackground(BoxBackground::Image(projection))
            );
        }
    
    private:
        void InitSideBar()
        {
            m_SideBar = std::make_shared<Box>();

            m_SideBar->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(320), AxisSizingRule::Fill())
                    .WithDirection(BoxDirection::Column)
                    .WithPadding(BoxPadding(0, 0, 0, 1))
                    .WithBackground(BoxBackground::Solid(ColorRGB(70, 70, 70)))
                    .WithGap(1)
            );

            m_SideBar->AddChild(std::make_shared<ColorSection>(m_ColorPalette));
            m_SideBar->AddChild(std::make_shared<LayerSection>(m_Project));
        }

        void InitViewport()
        {
            m_Viewport = std::make_shared<Box>();
            m_ViewportPreview = std::make_shared<Box>();
            m_ViewportOverlay = std::make_shared<Box>();

            SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())\
                    .WithBackground(BoxBackground::Solid(ColorRGB(255, 0, 0)))
            );

            m_Viewport->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center)
                    .WithBackground(BoxBackground::Solid(ColorRGB(30, 30, 30)))
            );

            m_ViewportPreview->SetStyle(
                StyleSheet()
                    .WithBackgroundReference(
                        BoxBackgroundTransparencyReference::Checkerboard(
                            ColorRGB(230, 230, 230),
                            ColorRGB(255, 255, 255),
                            8
                        )
                    )
            );

            m_ViewportOverlay->SetStyle(
                StyleSheet()
                    .WithSize(SizingRule(AxisSizingRule::Fill(), AxisSizingRule::Fill()))
                    .WithPosition(PositioningRule::Relative(Vec2(0.0f, 0.0f)))
            );
    
            m_Viewport->AddChild(m_ViewportPreview);
            m_Viewport->AddChild(m_ViewportOverlay);
        }

        void InitToolBar()
        {
            m_ToolBar = std::make_shared<Box>();

            m_ToolBarTools = std::make_shared<Box>();
            m_ToolBarDivider = std::make_shared<Box>();

            m_ToolBar->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fit(), AxisSizingRule::Fill())
            );

            m_ToolBarTools->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fit(), AxisSizingRule::Fill())
                    .WithDirection(BoxDirection::Column)
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Start)
                    .WithBackground(BoxBackground::Solid(ColorRGB(44, 44, 44)))
                    .WithPadding(BoxPadding(8))
                    .WithGap(8)
            );

            m_ToolBarDivider->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(1), AxisSizingRule::Fill())
                    .WithBackground(BoxBackground::Solid(ColorRGB(70, 70, 70)))
            );

            m_ToolBar->AddChild(m_ToolBarTools);
            m_ToolBar->AddChild(m_ToolBarDivider);
        }

        void InitToolButton(const std::shared_ptr<Bitmap>& icon, const std::shared_ptr<Tool>& tool)
        {
            auto button = std::make_shared<Box>();

            button->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(40), AxisSizingRule::Fixed(40))
                    .WithBackground(BoxBackground::Image(icon))
                    .WithBackgroundReference(BoxBackgroundTransparencyReference::Static(ColorRGB(44, 44, 44)))
                    .WithPadding(BoxPadding(8))
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

            button->OnMousePress = [this, tool](Element& element)
            {
                for (auto& child : m_ToolBarTools->GetChildren())
                {
                    child->DisableTrait("selected");
                }

                m_ViewportOverlay->ClearChildren();
                m_ViewportOverlay->AddChild(tool->CreateOverlay());

                element.EnableTrait("selected");
            };

            m_ToolBarTools->AddChild(button);
        }
    };
}