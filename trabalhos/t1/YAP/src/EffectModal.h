#pragma once

#include "Modal.h"
#include "Effects.h"

namespace yap
{
    class EffectModal : public Modal
    {
    private:
        std::shared_ptr<Project> m_Project;
        std::shared_ptr<Layer> m_WorkLayer;

        std::shared_ptr<Bitmap> m_PreviewBitmap;

        std::vector<std::shared_ptr<Effect>> m_Effects;

        int m_CurrentEffectIndex = -1;

        std::shared_ptr<Box> m_Header;
        std::shared_ptr<Box> m_Body;

        std::shared_ptr<Box> m_CurrentEffectOptions;
        std::shared_ptr<Text> m_CurrentEffectName;

    public:
        EffectModal(const std::shared_ptr<Project>& project) : m_Project(project)
        {
            m_WorkLayer = m_Project->GetActiveLayer();
            m_Effects = {
                std::make_shared<BrightnessContrastEffect>(),
                std::make_shared<GrayscaleEffect>(),
                std::make_shared<PixelateEffect>(),
                std::make_shared<GaussianBlurEffect>()
            };

            m_PreviewBitmap = std::make_shared<Bitmap>();

            m_Header = std::make_shared<Box>();
            m_Body = std::make_shared<Box>();
            
            m_CurrentEffectName = std::make_shared<Text>();
            m_CurrentEffectOptions = std::make_shared<Box>();

            InitHeader();
            InitBody();

            SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(600), AxisSizingRule::Fit())
                    .WithDirection(BoxDirection::Column)
                    .WithPadding(BoxPadding(1))
                    .WithGap(1)
                    .WithForeground(ColorRGB(255, 255, 255))
                    .WithBackground(BoxBackground::Solid(ColorRGB(70, 70, 70)))
            );

            AddChild(m_Header);
            AddChild(m_Body);
        }

    private:
        void InitHeader()
        {
            auto headerTitle = std::make_shared<Text>();
            auto headerFill = std::make_shared<Box>();

            auto headerCloseButton = CreateIconButton(
                std::make_shared<Bitmap>(BMP::Load("YAP/assets/x.bmp")),
                [this](Element& element)
                {
                    Close();
                }
            );

            headerTitle->Content = "Efeitos";

            headerFill->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
            );

            m_Header->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithAlignment(BoxAxisAlignment::Start, BoxAxisAlignment::Center)
                    .WithPadding(BoxPadding(8, 8, 8, 16))
                    .WithBackground(BoxBackground::Solid(ColorRGB(44, 44, 44)))
            );

            m_Header->AddChild(headerTitle);
            m_Header->AddChild(headerFill);
            m_Header->AddChild(headerCloseButton);
        }

        void InitBody()
        {
            m_Body->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithDirection(BoxDirection::Column)
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center)
                    .WithPadding(BoxPadding(8))
                    .WithGap(16)
                    .WithBackground(BoxBackground::Solid(ColorRGB(44, 44, 44)))
            );

            if (!m_WorkLayer)
            {
                auto errorText = std::make_shared<Text>();
                auto okButton = CreateTextButton("OK", [this](Element& element)
                {
                    Close();
                });

                errorText->Content = "Selecione uma camada para aplicar efeitos.";

                m_Body->AddChild(errorText);
                m_Body->AddChild(okButton);

                return;
            }

            auto preview = std::make_shared<Box>();

            auto carouselSelection = std::make_shared<Box>();

            auto carouselPreviousButton = CreateIconButton(
                std::make_shared<Bitmap>(BMP::Load("YAP/assets/chevron-left.bmp")),
                [this](Element& element)
                {
                    PreviousEffect();
                }
            );

            auto carouselNextButton = CreateIconButton(
                std::make_shared<Bitmap>(BMP::Load("YAP/assets/chevron-right.bmp")),
                [this](Element& element)
                {
                    NextEffect();
                }
            );

            auto carousel = std::make_shared<Box>();

            auto cancelButton = CreateTextButton("Cancelar", [this](Element& element)
            {
                Close();
            });

            auto applyButton = CreateTextButton("Aplicar", [this](Element& element)
            {
                m_WorkLayer->SetBitmap(*m_PreviewBitmap);
                Close();
            });

            auto buttons = std::make_shared<Box>();

            preview->SetStyle(
                StyleSheet()
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center)
                    .WithBackground(BoxBackground::Image(m_PreviewBitmap))
                    .WithBackgroundReference(
                        BoxBackgroundTransparencyReference::Checkerboard(
                            ColorRGB(230, 230, 230),
                            ColorRGB(255, 255, 255),
                            8
                        )
                    )
                    .WithBackgroundPosition(BoxBackgroundPositioningRule::Center())
            );

            preview->OnAnimate = [this](Element& element)
            {
                int maximumWidth = 384;
                int maximumHeight = 216;

                int layerWidth = m_WorkLayer->GetBitmap()->GetWidth();
                int layerHeight = m_WorkLayer->GetBitmap()->GetHeight();

                float aspectRatio = static_cast<float>(layerWidth) / static_cast<float>(layerHeight);

                int previewWidth = maximumWidth;
                int previewHeight = static_cast<int>(maximumWidth / aspectRatio);

                if (previewHeight > maximumHeight)
                {
                    previewHeight = maximumHeight;
                    previewWidth = static_cast<int>(maximumHeight * aspectRatio);
                }

                element.SetStyle(
                    element.GetStyle()
                        .WithSize(
                            AxisSizingRule::Fixed(previewWidth),
                            AxisSizingRule::Fixed(previewHeight)
                        )
                );
            };

            carouselSelection->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center)
            );

            carouselSelection->AddChild(m_CurrentEffectName);

            carousel->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center)
                    .WithBackground(BoxBackground::Solid(ColorRGB(44, 44, 44)))
            );

            carousel->AddChild(carouselPreviousButton);
            carousel->AddChild(carouselSelection);
            carousel->AddChild(carouselNextButton);

            m_CurrentEffectOptions->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(280))
            );

            buttons->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center)
                    .WithGap(8)
            );

            buttons->AddChild(cancelButton);
            buttons->AddChild(applyButton);

            m_Body->OnMount = [this](Element& element)
            {
                SelectEffect(0);
            };

            m_Body->AddChild(carousel);
            m_Body->AddChild(preview);
            m_Body->AddChild(m_CurrentEffectOptions);
            m_Body->AddChild(buttons);
        }

        std::shared_ptr<Box> CreateIconButton(const std::shared_ptr<Bitmap>& icon, const std::function<void(Element&)>& onPress)
        {
            auto button = std::make_shared<Box>();

            button->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(40), AxisSizingRule::Fixed(40))
                    .WithBackground(BoxBackground::Image(icon))
                    .WithBackgroundReference(BoxBackgroundTransparencyReference::Static(ColorRGB(44, 44, 44)))
            );

            button->SetStyle(
                ":hover",
                StyleSheet()
                    .WithBackgroundReference(BoxBackgroundTransparencyReference::Static(ColorRGB(56, 56, 56)))
            );

            button->OnMousePress = [onPress](Element& element)
            {
                onPress(element);
            };

            return button;
        }

        std::shared_ptr<Box> CreateTextButton(const std::string& text, const std::function<void(Element&)>& onPress)
        {
            auto button = std::make_shared<Box>();
            auto buttonText = std::make_shared<Text>();

            buttonText->Content = text;

            button->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(40))
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center)
                    .WithPadding(BoxPadding(8))
            );

            button->SetStyle(
                ":hover",
                StyleSheet()
                    .WithBackground(BoxBackground::Solid(ColorRGB(56, 56, 56)))
            );

            button->OnMousePress = [onPress](Element& element)
            {
                onPress(element);
            };

            button->AddChild(buttonText);

            return button;
        }

        void NextEffect()
        {
            SelectEffect(m_CurrentEffectIndex + 1);
        }

        void PreviousEffect()
        {
            SelectEffect(m_CurrentEffectIndex - 1);
        }

        void SelectEffect(int index)
        {
            if (m_CurrentEffectIndex != -1)
            {
                m_Effects[m_CurrentEffectIndex]->OnUpdate = nullptr;
            }

            m_CurrentEffectIndex = (index + m_Effects.size()) % m_Effects.size();

            RefreshCurrentEffectDetails();
            RenderCurrentEffectPreviewBitmap();

            m_Effects[m_CurrentEffectIndex]->OnUpdate = [this](Effect& effect)
            {
                RenderCurrentEffectPreviewBitmap();
            };
        }

        void RefreshCurrentEffectDetails()
        {
            m_CurrentEffectName->Content = m_Effects[m_CurrentEffectIndex]->CreateName();
            m_CurrentEffectOptions->ClearChildren();
            m_CurrentEffectOptions->AddChild(m_Effects[m_CurrentEffectIndex]->CreateOptions());
        }

        void RenderCurrentEffectPreviewBitmap()
        {
            auto effect = m_Effects[m_CurrentEffectIndex];

            effect->Apply(*m_WorkLayer->GetBitmap(), *m_PreviewBitmap);
        }
    };
}