#pragma once

#include "Modal.h"
#include "Effects.h"

/**
 * @file EffectModal.h
 * @brief Defines the EffectModal class, which provides a user interface for applying visual effects to a layer in a project.
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
        std::shared_ptr<Project> m_Project; ///< The project associated with this modal.
        std::shared_ptr<Layer> m_WorkLayer; ///< The active layer to which effects will be applied.

        std::shared_ptr<Bitmap> m_PreviewBitmap; ///< A bitmap used to preview the effect.

        std::vector<std::shared_ptr<Effect>> m_Effects; ///< List of available effects.

        int m_CurrentEffectIndex = -1; ///< Index of the currently selected effect.

        std::shared_ptr<Box> m_CurrentEffectOptions; ///< UI container for the current effect's options.
        std::shared_ptr<Text> m_CurrentEffectName; ///< UI element displaying the name of the current effect.

    public:
        /**
         * @brief Constructs an EffectModal instance.
         * @param project The project to which this modal belongs.
         */
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
        /**
         * @brief Selects the next effect in the list.
         */
        void NextEffect()
        {
            SelectEffect(m_CurrentEffectIndex + 1);
        }

        /**
         * @brief Selects the previous effect in the list.
         */
        void PreviousEffect()
        {
            SelectEffect(m_CurrentEffectIndex - 1);
        }

        /**
         * @brief Selects an effect by its index.
         * @param index The index of the effect to select.
         */
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

        /**
         * @brief Updates the UI to reflect the details of the currently selected effect.
         */
        void RefreshCurrentEffectDetails()
        {
            m_CurrentEffectName->Content = m_Effects[m_CurrentEffectIndex]->CreateName();
            m_CurrentEffectOptions->ClearChildren();
            m_CurrentEffectOptions->AddChild(m_Effects[m_CurrentEffectIndex]->CreateOptions());
        }

        /**
         * @brief Renders the preview bitmap for the currently selected effect.
         */
        void RenderCurrentEffectPreviewBitmap()
        {
            auto effect = m_Effects[m_CurrentEffectIndex];

            effect->Apply(*m_WorkLayer->GetBitmap(), *m_PreviewBitmap);
        }
    };
}