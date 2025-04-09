#pragma once

#include "Math.h"

#include "Box.h"
#include "Screen.h"

namespace yap
{
    class Slider : public Box
    {
    private:
        std::shared_ptr<Box> m_Track;
        std::shared_ptr<Box> m_Thumb;

        float m_Value = 0.0f;

    public:
        float MinValue = 0.0f;
        float MaxValue = 1.0f;

        float Step = 0.01f;

        std::function<void(Slider&, float)> OnChange;
        std::function<void(Slider&, float)> OnChangeEnd;

        Slider()
        {
            m_Track = std::make_shared<Box>();
            m_Thumb = std::make_shared<Box>();

            InitTrack();
            InitThumb();

            SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithPadding(BoxPadding(0, 2))
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

            OnMouseRelease = [this](Element& element) {
                if (OnChangeEnd)
                {
                    OnChangeEnd(*this, m_Value);
                }
            };

            AddChild(m_Track);
            AddChild(m_Thumb);
        }

        void SetValue(float value)
        {
            m_Value = std::floor(value / Step) * Step;
            m_Value = Clamp(m_Value, MinValue, MaxValue);
        }

        float GetValue() const
        {
            return m_Value;
        }

    private:
        void InitTrack()
        {
            m_Track->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(12))
                    .WithBackground(BoxBackground::Solid(ColorRGB(56, 56, 56)))
            );
        }

        void InitThumb()
        {
            m_Thumb->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(16), AxisSizingRule::Fixed(16))
                    .WithBackground(BoxBackground::Solid(ColorRGB(255, 255, 255)))
                    .WithPosition(PositioningRule::Relative(Vec2(0.0f, 0.0f)))
            );

            m_Thumb->OnMousePress = [this](Element& element) {
                SyncThumbToMouse();
            };

            m_Thumb->OnMouseMove = [this](Element& element) {
                if (element.IsPressed())
                {
                    SyncThumbToMouse();
                }
            };

            m_Thumb->OnMouseRelease = [this](Element& element) {
                if (OnChangeEnd)
                {
                    OnChangeEnd(*this, m_Value);
                }
            };
        }

        void SyncThumbToMouse()
        {
            const Mouse& mouse = GetScreen()->GetMouse();

            float offset = (mouse.Position.X - m_Track->Position.X) / m_Track->Size.X;

            SetValue((MaxValue - MinValue) * offset + MinValue);

            if (OnChange)
            {
                OnChange(*this, m_Value);
            }
        }

        void RefreshThumb()
        {
            float offset = Clamp((m_Value - MinValue) / (MaxValue - MinValue), 0.0f, 1.0f);

            Vec2 position = Vec2(offset, 0.0f);
            position *= m_Track->Size;
            position -= Vec2(8.0, 0.0);

            m_Thumb->SetStyle(
                m_Thumb->GetStyle()
                    .WithPosition(PositioningRule::Relative(position))
            );
        }
    };
}