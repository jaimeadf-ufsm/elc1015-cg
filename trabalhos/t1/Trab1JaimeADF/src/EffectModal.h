#pragma once

#include "Modal.h"
#include "Effects.h"

/**
 * @file EffectModal.h
 * @brief Defines the EffectModal class, which provides a UI component for applying visual effects to a layer in a project.
 */

namespace yap
{
    /**
     * @class EffectModal
     * @brief A modal dialog for managing and applying visual effects to a layer in a project.
     */
    class EffectModal : public Modal
    {
    private:
        std::shared_ptr<Project> m_Project;
        std::shared_ptr<Layer> m_WorkLayer;

        std::shared_ptr<Bitmap> m_PreviewBitmap;

        std::vector<std::shared_ptr<Effect>> m_Effects;

        int m_CurrentEffectIndex = -1;

        std::shared_ptr<Box> m_CurrentEffectOptions;
        std::shared_ptr<Text> m_CurrentEffectName;

    public:
        EffectModal(const std::shared_ptr<Project>& project) : m_Project(project)
        {
            m_WorkLayer = m_Project->GetActiveLayer();
            m_Effects = {
                std::make_shared<BrightnessContrastEffect>(),
                std::make_shared<GammaCorrectionEffect>(),
                std::make_shared<GrayscaleEffect>(),
                std::make_shared<SepiaEffect>(),
                std::make_shared<GaussianBlurEffect>(),
                std::make_shared<PixelateEffect>(),
                std::make_shared<RandomNoiseEffect>()
            };

            m_PreviewBitmap = std::make_shared<Bitmap>();

            m_CurrentEffectOptions = std::make_shared<Box>();
            m_CurrentEffectName = std::make_shared<Text>();

            auto header = CreateHeader("Efeitos");
            auto body = CreateBody();

            m_CurrentEffectOptions->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(280))
            );

            if (m_WorkLayer)
            {
                auto preview = std::make_shared<Box>();

                auto carousel = std::make_shared<Box>();

                auto carouselSelection = std::make_shared<Box>();

                auto carouselPreviousButton = CreateIconButton(std::make_shared<Bitmap>(BMP::Load("Trab1JaimeADF/assets/chevron-left-40x40.bmp")));
                auto carouselNextButton = CreateIconButton(std::make_shared<Bitmap>(BMP::Load("Trab1JaimeADF/assets/chevron-right-40x40.bmp")));

                auto buttons = std::make_shared<Box>();

                auto cancelButton = CreateTextButton("Cancelar");
                auto applyButton = CreateTextButton("Aplicar");

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

                carouselPreviousButton->OnMousePress = [this](Element& element)
                {
                    PreviousEffect();
                };

                carouselNextButton->OnMousePress = [this](Element& element)
                {
                    NextEffect();
                };

                carousel->SetStyle(
                    StyleSheet()
                        .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                        .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center)
                        .WithBackground(BoxBackground::Solid(ColorRGB(44, 44, 44)))
                );

                carousel->AddChild(carouselPreviousButton);
                carousel->AddChild(carouselSelection);
                carousel->AddChild(carouselNextButton);

                cancelButton->OnMousePress = [this](Element& element)
                {
                    Close();
                };

                applyButton->OnMousePress = [this](Element& element)
                {
                    m_WorkLayer->SetBitmap(*m_PreviewBitmap);
                    Close();
                };

                buttons->SetStyle(
                    StyleSheet()
                        .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                        .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center)
                        .WithGap(8)
                );

                buttons->AddChild(cancelButton);
                buttons->AddChild(applyButton);

                body->OnMount = [this](Element& element)
                {
                    SelectEffect(0);
                };

                body->AddChild(carousel);
                body->AddChild(preview);
                body->AddChild(m_CurrentEffectOptions);
                body->AddChild(buttons);
            } 
            else
            {
                auto errorText = std::make_shared<Text>();
                auto okButton = CreateTextButton("OK");

                errorText->Content = "Selecione uma camada para aplicar efeitos.";

                okButton->OnMousePress = [this](Element& element)
                {
                    Close();
                };

                body->AddChild(errorText);
                body->AddChild(okButton);
            }

            AddChild(header);
            AddChild(body);
        }

    private:
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