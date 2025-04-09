#pragma once

#include <iomanip>
#include <sstream>

#include "Bitmap.h"
#include "Box.h"
#include "Text.h"

namespace yap
{
    class Effect
    {
    private:
        std::string m_Name;
    
    public:
        std::function<void(Effect&)> OnUpdate;

        Effect(const std::string& name) : m_Name(name)
        {
            m_Name = name;
        }

        virtual const std::string& CreateName()
        {
            return m_Name;
        }

        virtual std::shared_ptr<Box> CreateOptions()
        {
            auto form = CreateForm();

            auto warningText = std::make_shared<Text>();
            auto warningWrapper = std::make_shared<Box>();

            warningText->Content = "Este efeito carece de ajustes.";

            warningWrapper->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center)
            );

            warningWrapper->AddChild(warningText);
            form->AddChild(warningWrapper);

            return form;
        }

        virtual void Apply(const Bitmap& source, Bitmap& destination) = 0;

    protected:
        std::shared_ptr<Box> CreateForm()
        {
            auto container = std::make_shared<Box>();

            container->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithDirection(BoxDirection::Column)
                    .WithPadding(BoxPadding(8, 0))
                    .WithGap(8)
            );

            return container;
        }

        std::shared_ptr<Text> CreateLabel(const std::string& text)
        {
            auto label = std::make_shared<Text>();

            label->Content = text;

            return label;
        }

        std::shared_ptr<Box> CreateSpacing(float size = 8.0f)
        {
            auto box = std::make_shared<Box>();

            box->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(size))
            );

            return box;
        }

        std::shared_ptr<Box> CreateSlider(
            float min,
            float max,
            float step,
            float defaultValue,
            std::function<void(float)> handleChange,
            std::function<std::string(float)> formatValue = [](float value) { return std::to_string(value); }
        )
        {
            auto container = std::make_shared<Box>();
            auto slider = std::make_shared<Slider>();

            auto textWrapper = std::make_shared<Box>();
            auto text = std::make_shared<Text>();

            container->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithAlignment(BoxAxisAlignment::Start, BoxAxisAlignment::Center)
                    .WithDirection(BoxDirection::Row)
                    .WithGap(16)
            );

            slider->MinValue = min;
            slider->MaxValue = max;
            slider->Step = step;

            slider->SetValue(defaultValue);

            slider->OnChange = [this, text, formatValue](Slider& slider, float value)
            {
                text->Content = formatValue(value);
            };

            slider->OnChangeEnd = [this, text, handleChange, formatValue](Slider& slider, float value)
            {
                handleChange(value);
            };

            text->Content = formatValue(defaultValue);

            textWrapper->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(60), AxisSizingRule::Fit())
                    .WithAlignment(BoxAxisAlignment::End, BoxAxisAlignment::Center)
            );

            textWrapper->AddChild(text);

            container->AddChild(slider);
            container->AddChild(textWrapper);

            return container;
        }

        void TriggerUpdate()
        {
            if (OnUpdate)
            {
                OnUpdate(*this);
            }
        }
    };

    class BrightnessContrastEffect : public Effect
    {
    private:
        float m_Brightness = 0.0f;
        float m_Contrast = 0.0f;

    public:
        BrightnessContrastEffect() : Effect("Brilho / Contraste")
        {
        }

        std::shared_ptr<Box> CreateOptions() override
        {
            auto form = CreateForm();

            form->AddChild(CreateLabel("Brilho:"));
            form->AddChild(CreateSlider(
                -1.0f,
                1.0f,
                0.01f,
                m_Brightness,
                [this](float value) {
                    m_Brightness = value;
                    TriggerUpdate();
                },
                [](float value) {
                    return std::to_string(static_cast<int>(value * 100.0f)) + "%";
                })
            );

            form->AddChild(CreateSpacing(8.0f));

            form->AddChild(CreateLabel("Contraste:"));
            form->AddChild(CreateSlider(
                -1.0f,
                1.0f,
                0.01f,
                m_Contrast,
                [this](float value) {
                    m_Contrast = value;
                    TriggerUpdate();
                },
                [](float value) {
                    return std::to_string(static_cast<int>(value * 100.0f)) + "%";
                })
            );

            return form;
        }

        void Apply(const Bitmap& source, Bitmap& destination) override
        {
            destination.Reallocate(source.GetWidth(), source.GetHeight());

            for (int y = 0; y < source.GetHeight(); ++y)
            {
                for (int x = 0; x < source.GetWidth(); ++x)
                {
                    ColorRGBA color = source.GetPixel(x, y);

                    color.R = Clamp(color.R + m_Brightness, 0.0f, 1.0f);
                    color.G = Clamp(color.G + m_Brightness, 0.0f, 1.0f);
                    color.B = Clamp(color.B + m_Brightness, 0.0f, 1.0f);

                    color.R = Clamp((color.R - 0.5f) * (1.0f + m_Contrast) + 0.5f, 0.0f, 1.0f);
                    color.G = Clamp((color.G - 0.5f) * (1.0f + m_Contrast) + 0.5f, 0.0f, 1.0f);
                    color.B = Clamp((color.B - 0.5f) * (1.0f + m_Contrast) + 0.5f, 0.0f, 1.0f);

                    destination.SetPixel(x, y, color);
                }
            }
        }
    };

    class GrayscaleEffect : public Effect
    {
    public:
        GrayscaleEffect() : Effect("Tons de Cinza")
        {
        }

        void Apply(const Bitmap& source, Bitmap& destination) override
        {
            destination.Reallocate(source.GetWidth(), source.GetHeight());

            for (int y = 0; y < source.GetHeight(); ++y)
            {
                for (int x = 0; x < source.GetWidth(); ++x)
                {
                    ColorRGBA color = source.GetPixel(x, y);

                    float gray = 0.3f * color.R + 0.59f * color.G + 0.11f * color.B;

                    destination.SetPixel(x, y, ColorRGBA(gray, gray, gray, color.A));
                }
            }
        }
    };

    class PixelateEffect : public Effect
    {
    private:
        int m_BlockSize = 8;

    public:
        PixelateEffect() : Effect("Pixelar")
        {
        }

        std::shared_ptr<Box> CreateOptions() override
        {
            auto form = CreateForm();

            form->AddChild(CreateLabel("Tamanho do bloco:"));
            form->AddChild(CreateSlider(
                1.0f,
                64.0f,
                1.0f,
                static_cast<float>(m_BlockSize),
                [this](float value) {
                    m_BlockSize = static_cast<int>(value);
                    TriggerUpdate();
                },
                [](float value) {
                    return std::to_string(static_cast<int>(value)) + " px";
                })
            );

            return form;
        }

        void Apply(const Bitmap& source, Bitmap& destination) override
        {
            destination.Reallocate(source.GetWidth(), source.GetHeight());

            for (int y = 0; y < source.GetHeight(); y += m_BlockSize)
            {
                for (int x = 0; x < source.GetWidth(); x += m_BlockSize)
                {
                    ColorRGBA averageColor = ColorRGBA(0, 0, 0, 0);
                    int count = 0;

                    for (int j = 0; j < m_BlockSize && y + j < source.GetHeight(); ++j)
                    {
                        for (int i = 0; i < m_BlockSize && x + i < source.GetWidth(); ++i)
                        {
                            averageColor += source.GetPixel(x + i, y + j);
                            count++;
                        }
                    }

                    averageColor /= static_cast<float>(count);

                    for (int j = 0; j < m_BlockSize && y + j < source.GetHeight(); ++j)
                    {
                        for (int i = 0; i < m_BlockSize && x + i < source.GetWidth(); ++i)
                        {
                            destination.SetPixel(x + i, y + j, averageColor);
                        }
                    }
                }
            }
        }
    };

    class GaussianBlurEffect : public Effect
    {
    private:
        float m_Radius = 1.0f;

    public:
        GaussianBlurEffect() : Effect("Desfoque Gaussiano")
        {
        }

        std::shared_ptr<Box> CreateOptions() override
        {
            auto form = CreateForm();

            form->AddChild(CreateLabel("Raio do desfoque:"));
            form->AddChild(CreateSlider(
                0.1f,
                10.0f,
                0.1f,
                m_Radius,
                [this](float value) {
                    m_Radius = value;
                    TriggerUpdate();
                },
                [](float value) {
                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(1) << value;
                    return ss.str();
                })
            );

            return form;
        }

        void Apply(const Bitmap& source, Bitmap& destination) override
        {
            destination.Reallocate(source.GetWidth(), source.GetHeight());
            
            Bitmap temp(source.GetWidth(), source.GetHeight());
            
            int kernelSize = 2 * std::max(1, static_cast<int>(2.0f * m_Radius)) + 1;
            int halfSize = kernelSize / 2;
            
            std::vector<float> kernel(kernelSize);
            float sigma = m_Radius;
            float sum = 0.0f;
            
            for (int i = 0; i < kernelSize; i++) {
                float x = i - halfSize;
                kernel[i] = std::exp(-(x * x) / (2 * sigma * sigma));
                sum += kernel[i];
            }
            
            for (int i = 0; i < kernelSize; i++) {
                kernel[i] /= sum;
            }
            
            for (int y = 0; y < source.GetHeight(); ++y) {
                for (int x = 0; x < source.GetWidth(); ++x) {
                    ColorRGBA blurredColor(0, 0, 0, 0);
                    
                    for (int i = 0; i < kernelSize; ++i) {
                        int sampleX = x + (i - halfSize);
                        
                        sampleX = std::max(0, std::min(source.GetWidth() - 1, sampleX));
                        
                        ColorRGBA sampleColor = source.GetPixel(sampleX, y);
                        blurredColor += sampleColor * kernel[i];
                    }
                    
                    temp.SetPixel(x, y, blurredColor);
                }
            }
            
            for (int y = 0; y < temp.GetHeight(); ++y) {
                for (int x = 0; x < temp.GetWidth(); ++x) {
                    ColorRGBA blurredColor(0, 0, 0, 0);
                    
                    for (int i = 0; i < kernelSize; ++i) {
                        int sampleY = y + (i - halfSize);
                        
                        // Clamp to edge
                        sampleY = std::max(0, std::min(temp.GetHeight() - 1, sampleY));
                        
                        ColorRGBA sampleColor = temp.GetPixel(x, sampleY);
                        blurredColor += sampleColor * kernel[i];
                    }
                    
                    destination.SetPixel(x, y, blurredColor);
                }
            }
        }
    };
}