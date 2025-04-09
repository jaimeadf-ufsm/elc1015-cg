#pragma once

#include "Project.h"

#include "ModalStack.h"

#include "Tools.h"
#include "EffectModal.h"
#include "ColorSection.h"
#include "LayerSection.h"

namespace yap
{
    class Workspace : public Box
    {
    private:
        std::shared_ptr<Project> m_Project;
        std::shared_ptr<ColorPalette> m_ColorPalette;
        std::shared_ptr<ViewportSpace> m_ViewportSpace;

        std::shared_ptr<ModalStack> m_ModalStack;

        std::shared_ptr<Box> m_MainContent;
        std::shared_ptr<Box> m_ModalContent;

        std::shared_ptr<Box> m_Area;

        std::shared_ptr<Box> m_OptionsBar;

        std::shared_ptr<Box> m_Viewport;
        std::shared_ptr<Box> m_ViewportPreview;
        std::shared_ptr<Box> m_ViewportOverlay;

        std::shared_ptr<Box> m_ToolBar;
        std::shared_ptr<Box> m_ToolBarTools;
        std::shared_ptr<Box> m_ToolBarActions;

        std::shared_ptr<Box> m_SideBar;
    
    public:
        Workspace()
        {
            m_Project = std::make_shared<Project>(300, 300);
            m_ColorPalette = std::make_shared<ColorPalette>(ColorRGBA(255, 0, 0, 255));
            m_ModalStack = std::make_shared<ModalStack>();

            m_MainContent = std::make_shared<Box>();
            m_ModalContent = std::make_shared<Box>();

            m_Area = std::make_shared<Box>();
            m_OptionsBar = std::make_shared<Box>();
            m_Viewport = std::make_shared<Box>();
            m_ViewportPreview = std::make_shared<Box>();
            m_ViewportOverlay = std::make_shared<Box>();
            m_ToolBar = std::make_shared<Box>();
            m_ToolBarTools = std::make_shared<Box>();
            m_ToolBarActions = std::make_shared<Box>();
            m_SideBar = std::make_shared<Box>();

            m_ViewportSpace = std::make_shared<ViewportSpace>(m_Project, m_ViewportPreview);

            InitToolBar();
            InitArea();
            InitSideBar();

            InitOptionsBar();
            InitViewport();

            InitTool(
                std::make_shared<Bitmap>(BMP::Load("YAP/assets/move.bmp")),
                std::make_shared<MoveTool>(m_Project, m_ViewportSpace)
            );

            InitTool(
                std::make_shared<Bitmap>(BMP::Load("YAP/assets/transform.bmp")),
                std::make_shared<TransformTool>(m_Project, m_ViewportSpace)
            );

            InitTool(
                std::make_shared<Bitmap>(BMP::Load("YAP/assets/brush.bmp")),
                std::make_shared<BrushTool>(m_Project, m_ViewportSpace, std::make_shared<PencilBrush>(m_ColorPalette))
            );

            InitTool(
                std::make_shared<Bitmap>(BMP::Load("YAP/assets/eraser.bmp")),
                std::make_shared<BrushTool>(m_Project, m_ViewportSpace, std::make_shared<PencilBrush>(std::make_shared<ColorPalette>(ColorRGBA(0, 0, 0, 0))))
            );

            InitAction(
                std::make_shared<Bitmap>(BMP::Load("YAP/assets/horizontal-flip.bmp")),
                [this]()
                {
                    std::shared_ptr<Layer> layer = m_Project->GetActiveLayer();

                    if (layer)
                    {
                        layer->FlipHorizontally();
                    }
                }
            );

            InitAction(
                std::make_shared<Bitmap>(BMP::Load("YAP/assets/vertical-flip.bmp")),
                [this]()
                {
                    std::shared_ptr<Layer> layer = m_Project->GetActiveLayer();

                    if (layer)
                    {
                        layer->FlipVertically();
                    }
                }
            );

            InitAction(
                std::make_shared<Bitmap>(BMP::Load("YAP/assets/effects.bmp")),
                [this]()
                {
                    m_ModalStack->PushModal(std::make_shared<EffectModal>(m_Project));
                }
            );

            m_MainContent->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
                    .WithBackground(BoxBackground::Solid(ColorRGB(70, 70, 70)))
                    .WithGap(1)
            );

            m_MainContent->AddChild(m_ToolBar);
            m_MainContent->AddChild(m_Area);
            m_MainContent->AddChild(m_SideBar);

            m_ModalContent->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
                    .WithPosition(PositioningRule::Relative(Vec2(0.0f, 0.0f)))
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center)
            );

            m_ModalStack->m_OnModal = [this](ModalStack& stack, const std::shared_ptr<Modal>& modal)
            {
                auto screen = GetScreen();

                if (!screen)
                {
                    return;
                }

                screen->ExecuteNextFrame([this, modal]()
                {
                    m_ModalContent->ClearChildren();

                    m_MainContent->SetStyle(
                        m_MainContent->GetStyle()
                            .WithEvents(modal ? PointerEvents::None : PointerEvents::Auto)
                    );

                    if (modal)
                    {
                        m_ModalContent->AddChild(modal);
                    }
                });
            };

            SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
            );

            AddChild(m_MainContent);
            AddChild(m_ModalContent);
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
        void InitArea()
        {
            m_Area->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
                    .WithDirection(BoxDirection::Column)
                    .WithGap(1)
            );

            m_Area->AddChild(m_OptionsBar);
            m_Area->AddChild(m_Viewport);
        }

        void InitSideBar()
        {
            m_SideBar->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(320), AxisSizingRule::Fill())
                    .WithDirection(BoxDirection::Column)
                    .WithGap(1)
            );

            m_SideBar->AddChild(std::make_shared<ColorSection>(m_ColorPalette));
            m_SideBar->AddChild(std::make_shared<LayerSection>(m_Project));
        }

        void InitOptionsBar()
        {
            m_OptionsBar->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(56))
                    .WithBackground(BoxBackground::Solid(ColorRGB(44, 44, 44)))
                    .WithPadding(BoxPadding(8, 0))
            );
        }

        void InitViewport()
        {

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
            m_ToolBar->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fit(), AxisSizingRule::Fill())
                    .WithDirection(BoxDirection::Column)
                    .WithBackground(BoxBackground::Solid(ColorRGB(44, 44, 44)))
            );

            m_ToolBarTools->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fit(), AxisSizingRule::Fill())
                    .WithDirection(BoxDirection::Column)
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Start)
                    .WithPadding(BoxPadding(8))
                    .WithGap(8)
            );

            m_ToolBarActions->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fit(), AxisSizingRule::Fill())
                    .WithDirection(BoxDirection::Column)
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::End)
                    .WithPadding(BoxPadding(8))
                    .WithGap(8)
            );

            m_ToolBar->AddChild(m_ToolBarTools);
            m_ToolBar->AddChild(m_ToolBarActions);
        }

        void InitTool(const std::shared_ptr<Bitmap>& icon, const std::shared_ptr<Tool>& tool)
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

                m_OptionsBar->ClearChildren();
                m_OptionsBar->AddChild(tool->CreateOptions());

                element.EnableTrait("selected");
            };

            m_ToolBarTools->AddChild(button);
        }

        void InitAction(const std::shared_ptr<Bitmap>& icon, const std::function<void()>& action)
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

            button->OnMousePress = [action](Element& element)
            {
                action();
            };

            m_ToolBarActions->AddChild(button);
        }
    };
}