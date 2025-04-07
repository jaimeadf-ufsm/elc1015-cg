#pragma once

#include "Math.h"

#include "Box.h"
#include "Screen.h"

namespace yap
{
    class Slider : public Box
    {
    private:
        std::shared_ptr<Box> m_Thumb;
        std::shared_ptr<Box> m_Track;

        float m_Value = 0.0f;

    public:
        std::function<void(Slider&, float)> OnChange;

        Slider()
        {
            InitTrack();
            InitThumb();

            SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(16))
            );

            OnAnimate = [this](Element& element) {
                RefreshThumb();
            };

            OnMousePress = [this](Element& element) {
                SyncThumbToMouse();
            };

            OnMouseMove = [this](Element& element) {
                if (IsPressed())
                {
                    SyncThumbToMouse();
                }
            };
        }

        void SetValue(float value)
        {
            m_Value = Clamp(value, 0.0f, 1.0f);
        }

        float GetValue() const
        {
            return m_Value;
        }

    private:
        void InitTrack()
        {
            m_Track = std::make_shared<Box>();

            m_Track->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(12))
                    .WithBackground(BoxBackground::Solid(ColorRGB(56, 56, 56)))
            );

            AddChild(m_Track);
        }

        void InitThumb()
        {
            m_Thumb = std::make_shared<Box>();

            m_Thumb->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(16), AxisSizingRule::Fixed(16))
                    .WithBackground(BoxBackground::Solid(ColorRGB(255, 255, 255)))
            );

            AddChild(m_Thumb);
        }

        void SyncThumbToMouse()
        {
            const Mouse& mouse = GetScreen()->GetMouse();

            float value = (mouse.Position.X - m_Track->Position.X) / m_Track->Size.X;

            SetValue(value);

            if (OnChange)
            {
                OnChange(*this, m_Value);
            }
        }

        void RefreshThumb()
        {
            Vec2 position = Vec2(m_Value, 0.0f);
            position *= m_Track->Size;
            position += m_Track->Position;
            position -= Vec2(8.0f, 2.0f);

            m_Thumb->SetStyle(
                m_Thumb->GetStyle()
                    .WithPosition(PositioningRule::Absolute(position))
            );
        }
    };
}