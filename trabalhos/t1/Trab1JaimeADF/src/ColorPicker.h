/*
 * Summary:
 * This file defines a set of classes for creating a graphical color picker component. 
 * The `ColorPicker` class provides a user interface for selecting colors in HSVA (Hue, Saturation, Value, Alpha) format. 
 * It includes specialized components such as `SaturationPad`, `HuePad`, and `AlphaPad` for adjusting individual color properties.
 */

#pragma once

#include "Box.h"
#include "Text.h"

namespace yap
{
    /**
     * @class ColorPad
     * @brief Base class for interactive color adjustment pads.
     * 
     * Provides a foundation for creating color adjustment components such as saturation, hue, and alpha sliders.
     */
    class ColorPad : public Box
    {
    protected:
        ColorHSVA m_Color; ///< Current color value in HSVA format.
        std::shared_ptr<Bitmap> m_AreaBackground; ///< Bitmap for rendering the pad's background.
        std::shared_ptr<Box> m_Area; ///< The interactive area of the pad.
        std::shared_ptr<Box> m_Thumb; ///< The draggable thumb indicator.

    public:
        std::function<void(ColorPad&, const ColorHSVA&)> OnChange; ///< Callback triggered when the color changes.

        /**
         * @brief Constructor. Initializes the color pad and its components.
         */
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

        /**
         * @brief Sets the current color of the pad.
         * @param color The new color in HSVA format.
         */
        void SetColor(const ColorHSVA& color)
        {
            m_Color = color;

            RefreshArea();
            RefreshThumb();
        }

        /**
         * @brief Gets the current color of the pad.
         * @return The current color in HSVA format.
         */
        ColorHSVA GetColor() const
        {
            return m_Color;
        }
        
    protected: 
        /**
         * @brief Updates the position and style of the thumb based on the current color.
         */
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

        /**
         * @brief Updates the background of the pad based on the current color.
         */
        virtual void RefreshArea()
        {
            m_AreaBackground->Reallocate(m_Area->Size.X, m_Area->Size.Y);
        }

        /**
         * @brief Converts a proportional position on the pad to a color.
         * @param position The proportional position (0.0 to 1.0).
         * @return The corresponding color in HSVA format.
         */
        virtual ColorHSVA ConvertProportionalPositionToColor(const Vec2& position) = 0;

        /**
         * @brief Converts a color to a proportional position on the pad.
         * @param color The color in HSVA format.
         * @return The corresponding proportional position (0.0 to 1.0).
         */
        virtual Vec2 ConvertColorToProportionalPosition(const ColorHSVA& color) = 0;

    private:
        /**
         * @brief Initializes the thumb component.
         */
        void InitThumb()
        {
            m_Thumb = std::make_shared<Box>();

            m_Thumb->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(16), AxisSizingRule::Fixed(16))
                    .WithBorder(BoxBorder::Solid(ColorRGB(255, 255, 255), 1.0f))
            );
        }

        /**
         * @brief Initializes the interactive area component.
         */
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

        /**
         * @brief Synchronizes the color with the mouse position when interacting with the pad.
         */
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

    /**
     * @class SaturationPad
     * @brief A color pad for adjusting saturation and value.
     */
    class SaturationPad : public ColorPad
    {
    public:
        /**
         * @brief Constructor. Initializes the saturation pad.
         */
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

    /**
     * @class HuePad
     * @brief A color pad for adjusting hue.
     */
    class HuePad : public ColorPad
    {
    public:
        /**
         * @brief Constructor. Initializes the hue pad.
         */
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

    /**
     * @class AlphaPad
     * @brief A color pad for adjusting alpha (transparency).
     */
    class AlphaPad : public ColorPad
    {
    private:
        std::shared_ptr<Bitmap> m_ThumbBackground; ///< Bitmap for rendering the thumb's background.

    public:
        /**
         * @brief Constructor. Initializes the alpha pad.
         */
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

    /**
     * @class ColorPicker
     * @brief A graphical component for selecting colors in HSVA format.
     */
    class ColorPicker : public Box
    {
    private:
        ColorHSVA m_Color; ///< Current color in HSVA format.
        std::shared_ptr<ColorPad> m_SaturationPad; ///< Pad for adjusting saturation and value.
        std::shared_ptr<ColorPad> m_HuePad; ///< Pad for adjusting hue.
        std::shared_ptr<ColorPad> m_AlphaPad; ///< Pad for adjusting alpha.

    public:
        std::function<void(ColorPicker&, const ColorHSVA&)> OnChange; ///< Callback triggered when the color changes.

        /**
         * @brief Constructor. Initializes the color picker and its components.
         */
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

        /**
         * @brief Sets the current color of the picker.
         * @param color The new color in HSVA format.
         */
        void SetColor(const ColorHSVA& color)
        {
            if (color == m_Color)
            {
                return;
            }

            m_Color = color;

            RefreshPads();
        }

        /**
         * @brief Gets the current color of the picker.
         * @return The current color in HSVA format.
         */
        ColorHSVA GetColor() const
        {
            return m_Color;
        }

    private:
        /**
         * @brief Updates the color and triggers the OnChange callback.
         * @param color The new color in HSVA format.
         */
        void ChangeColor(const ColorHSVA& color)
        {
            SetColor(color);

            if (OnChange)
            {
                OnChange(*this, m_Color);
            }
        }

        /**
         * @brief Refreshes the state of all pads to match the current color.
         */
        void RefreshPads()
        {
            m_SaturationPad->SetColor(m_Color);
            m_HuePad->SetColor(m_Color);
            m_AlphaPad->SetColor(m_Color);
        }
    };
}
