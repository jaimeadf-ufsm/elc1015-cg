#pragma once

#include "Box.h"
#include "Text.h"

namespace yap
{
    class ColorPad : public Box
    {
    protected:
        ColorHSVA m_Color;

        std::shared_ptr<Bitmap> m_AreaBackground;

        std::shared_ptr<Box> m_Area;
        std::shared_ptr<Box> m_Thumb;
    
    public:
        std::function<void(ColorPad&, const ColorHSVA&)> OnChange;

        ColorPad()
        {
            m_AreaBackground = std::make_shared<Bitmap>();

            InitArea();
            InitThumb();

            SetStyle(
                StyleSheet()
                    .WithPadding(BoxPadding(8))
            );

            OnMount = [this](Element& element)
            {
                RefreshArea();
                RefreshThumb();
            };

            OnMousePress = [this](Element& element)
            {
                SyncColorToMousePosition();
            };

            OnMouseMove = [this](Element& element)
            {
                if (element.IsPressed())
                {
                    SyncColorToMousePosition();
                }
            };

            AddChild(m_Area);
            AddChild(m_Thumb);
        }

        void SetColor(const ColorHSVA& color)
        {
            m_Color = color;

            RefreshArea();
            RefreshThumb();
        }

        ColorHSVA GetColor() const
        {
            return m_Color;
        }
        
    protected: 
        virtual void RefreshThumb()
        {
            Vec2 position = ConvertColorToProportionalPosition(m_Color);
            position *= m_Area->Size;
            position += m_Area->Position - Position;
            position -= Vec2(8, 8);

            m_Thumb->SetStyle(
                m_Thumb->GetStyle()
                    .WithPosition(PositioningRule::Relative(position))
            );
        }

        virtual void RefreshArea()
        {
            m_AreaBackground->Reallocate(m_Area->Size.X, m_Area->Size.Y);
        }

        virtual ColorHSVA ConvertProportionalPositionToColor(const Vec2& position) = 0;
        virtual Vec2 ConvertColorToProportionalPosition(const ColorHSVA& color) = 0;

    private:
        void InitThumb()
        {
            m_Thumb = std::make_shared<Box>();

            m_Thumb->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(16), AxisSizingRule::Fixed(16))
                    .WithBorder(BoxBorder::Solid(ColorRGB(255, 255, 255), 1.0f))
            );
        }

        void InitArea()
        {
            m_Area = std::make_shared<Box>();

            m_Area->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
                    .WithBackground(BoxBackground::Image(m_AreaBackground))
                    .WithBackgroundReference(
                        BoxBackgroundTransparencyReference::Checkerboard(
                            ColorRGB(230, 230, 230),
                            ColorRGB(255, 255, 255),
                            8
                        )
                    )
            );

            m_Area->OnAnimate = [this](Element& element)
            {
                if (m_AreaBackground->GetWidth() != element.Size.X || m_AreaBackground->GetHeight() != element.Size.Y)
                {
                    RefreshArea();
                    RefreshThumb();
                }
            };
        }

        void SyncColorToMousePosition()
        {
            const Mouse& mouse = GetScreen()->GetMouse();

            Vec2 position = mouse.Position - m_Area->Position;
            position /= m_Area->Size;
            position.Clamp(0.0f, 1.0f);

            SetColor(ConvertProportionalPositionToColor(position));

            if (OnChange)
            {
                OnChange(*this, m_Color);
            }
        }
    };

    class SaturationPad : public ColorPad
    {
    public:
        SaturationPad() : ColorPad()
        {
            SetStyle(
                GetStyle()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
                    .WithPadding(BoxPadding(8))
            );
        }
    
    protected:
        void RefreshThumb() override
        {
            ColorPad::RefreshThumb();

            m_Thumb->SetStyle(
                m_Thumb->GetStyle()
                    .WithBackground(BoxBackground::Solid(ColorHSV(m_Color).ToRGB()))
            );
        }

        void RefreshArea() override
        {
            ColorPad::RefreshArea();

            for (int y = 0; y < m_AreaBackground->GetHeight(); ++y)
            {
                for (int x = 0; x < m_AreaBackground->GetWidth(); ++x)
                {
                    float proportionalX = static_cast<float>(x) / m_AreaBackground->GetWidth();
                    float proportionalY = static_cast<float>(y) / m_AreaBackground->GetHeight();

                    ColorHSV color = ColorHSV(m_Color.H, proportionalX, 1.0f - proportionalY);

                    m_AreaBackground->SetPixel(x, y, color.ToRGB());
                }
            }
        }

        ColorHSVA ConvertProportionalPositionToColor(const Vec2& position) override
        {
            return ColorHSVA(m_Color.H, position.X, 1.0f - position.Y, m_Color.A);
        }

        Vec2 ConvertColorToProportionalPosition(const ColorHSVA& color) override
        {
            return Vec2(color.S, 1.0f - color.V);
        }
    };

    class HuePad : public ColorPad
    {
    public:
        HuePad() : ColorPad()
        {
            SetStyle(
                GetStyle()
                    .WithSize(AxisSizingRule::Fixed(16), AxisSizingRule::Fill())
                    .WithPadding(BoxPadding(2, 8))
            );
        }

    protected:
        void RefreshThumb() override
        {
            ColorPad::RefreshThumb();

            m_Thumb->SetStyle(
                m_Thumb->GetStyle()
                    .WithBackground(BoxBackground::Solid(ColorHSV(m_Color.H, 1.0f, 1.0f).ToRGB()))
            );
        }

        void RefreshArea() override
        {
            ColorPad::RefreshArea();

            for (int y = 0; y < m_AreaBackground->GetHeight(); ++y)
            {
                for (int x = 0; x < m_AreaBackground->GetWidth(); ++x)
                {
                    float proportionalY = static_cast<float>(y) / m_AreaBackground->GetHeight();

                    ColorHSV color = ColorHSV(proportionalY * 360.0f, 1.0f, 1.0f);

                    m_AreaBackground->SetPixel(x, y, color.ToRGB());
                }
            }
        }

        ColorHSVA ConvertProportionalPositionToColor(const Vec2& position) override
        {
            return ColorHSVA(360.0f * position.Y, m_Color.S, m_Color.V, m_Color.A);
        }

        Vec2 ConvertColorToProportionalPosition(const ColorHSVA& color) override
        {
            return Vec2(0.5f, color.H / 360.0f);
        }
    };

    class AlphaPad : public ColorPad
    {
    private:
        std::shared_ptr<Bitmap> m_ThumbBackground;

    public:
        AlphaPad()
        {
            m_ThumbBackground = std::make_shared<Bitmap>();

            m_Area->SetStyle(
                m_Area->GetStyle()
                    .WithBackgroundReference(
                        BoxBackgroundTransparencyReference::Checkerboard(
                            ColorRGB(230, 230, 230),
                            ColorRGB(255, 255, 255),
                            7
                        )
                    )
            );

            m_Thumb->SetStyle(
                m_Thumb->GetStyle()
                    .WithBackground(BoxBackground::Image(m_ThumbBackground))
                    .WithBackgroundReference(
                        BoxBackgroundTransparencyReference::Checkerboard(
                            ColorRGB(230, 230, 230),
                            ColorRGB(255, 255, 255),
                            8
                        )
                    )
            );

            SetStyle(
                GetStyle()
                    .WithSize(AxisSizingRule::Fixed(16), AxisSizingRule::Fill())
                    .WithPadding(BoxPadding(2, 8))
            );
        }

    protected:
        void RefreshThumb() override
        {
            ColorPad::RefreshThumb();

            int thumbWidth = m_Thumb->Size.X;
            int thumbHeight = m_Thumb->Size.Y;

            m_ThumbBackground->Reallocate(thumbWidth, thumbHeight);
            m_ThumbBackground->Clear(m_Color.ToRGBA());
        }

        void RefreshArea() override
        {
            ColorPad::RefreshArea();

            for (int y = 0; y < m_AreaBackground->GetHeight(); ++y)
            {
                for (int x = 0; x < m_AreaBackground->GetWidth(); ++x)
                {
                    float proportionalY = static_cast<float>(y) / m_AreaBackground->GetHeight();

                    ColorHSVA color = ColorHSVA(ColorHSV(m_Color), 1.0f - proportionalY);

                    m_AreaBackground->SetPixel(x, y, color.ToRGBA());
                }
            }
        }

        ColorHSVA ConvertProportionalPositionToColor(const Vec2& position) override
        {
            return ColorHSVA(m_Color.H, m_Color.S, m_Color.V, 1.0f - position.Y);
        }

        Vec2 ConvertColorToProportionalPosition(const ColorHSVA& color) override
        {
            return Vec2(0.5f, 1.0f - color.A);
        }
    };

    class ColorPicker : public Box
    {
    private:
        ColorHSVA m_Color;

        std::shared_ptr<ColorPad> m_SaturationPad;
        std::shared_ptr<ColorPad> m_HuePad;
        std::shared_ptr<ColorPad> m_AlphaPad;
    
    public:
        std::function<void(ColorPicker &element, const ColorHSVA&)> OnChange;

        ColorPicker() : m_Color(0.0f, 1.0f, 1.0f, 1.0f)
        {
            auto sliders = std::make_shared<Box>();

            m_SaturationPad = std::make_shared<SaturationPad>();
            m_HuePad = std::make_shared<HuePad>();
            m_AlphaPad = std::make_shared<AlphaPad>();

            SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
            );

            sliders->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fit(), AxisSizingRule::Fill())
                    .WithGap(8)
            );

            m_SaturationPad->OnChange = [this](ColorPad& pad, const ColorHSVA& color)
            {
                ChangeColor(color);
            };

            m_HuePad->OnChange = [this](ColorPad& pad, const ColorHSVA& color)
            {
                ChangeColor(color);
            };

            m_AlphaPad->OnChange = [this](ColorPad& pad, const ColorHSVA& color)
            {
                ChangeColor(color);
            };

            sliders->AddChild(m_HuePad);
            sliders->AddChild(m_AlphaPad);

            AddChild(m_SaturationPad);
            AddChild(sliders);

            OnMount = [this](Element& element)
            {
                RefreshPads();
            };
        }

        void SetColor(const ColorHSVA& color)
        {
            if (color == m_Color)
            {
                return;
            }

            m_Color = color;

            RefreshPads();
        }

        ColorHSVA GetColor() const
        {
            return m_Color;
        }

    private:
        void ChangeColor(const ColorHSVA& color)
        {
            SetColor(color);

            if (OnChange)
            {
                OnChange(*this, m_Color);
            }
        }

        void RefreshPads()
        {
            m_SaturationPad->SetColor(m_Color);
            m_HuePad->SetColor(m_Color);
            m_AlphaPad->SetColor(m_Color);
        }
    };
}
