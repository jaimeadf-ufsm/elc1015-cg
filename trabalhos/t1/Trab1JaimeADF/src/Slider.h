#pragma once

#include "Math.h"
#include "Box.h"
#include "Screen.h"

/**
 * @file Slider.h
 * @brief Defines the Slider class, a UI component for selecting a value within a range.
 * 
 * The Slider class provides a graphical user interface element that allows users to select
 * a value by moving a thumb along a track. It supports customization of range, step size,
 * and event handling for value changes.
 */

namespace yap
{
    /**
     * @class Slider
     * @brief A UI component for selecting a value within a specified range.
     * 
     * The Slider class inherits from Box and provides functionality for a track and a thumb
     * that can be moved to select a value. It supports event callbacks for value changes and
     * end-of-interaction events.
     */
    class Slider : public Box
    {
    private:
        std::shared_ptr<Box> m_Track; ///< The track of the slider.
        std::shared_ptr<Box> m_Thumb; ///< The thumb of the slider.
        float m_Value = 0.0f; ///< The current value of the slider.

    public:
        float MinValue = 0.0f; ///< The minimum value of the slider.
        float MaxValue = 1.0f; ///< The maximum value of the slider.
        float Step = 0.01f; ///< The step size for the slider value.

        /**
         * @brief Callback for when the slider value changes.
         * @param Slider& Reference to the slider instance.
         * @param float The new value of the slider.
         */
        std::function<void(Slider&, float)> OnChange;

        /**
         * @brief Callback for when the slider interaction ends.
         * @param Slider& Reference to the slider instance.
         * @param float The final value of the slider.
         */
        std::function<void(Slider&, float)> OnChangeEnd;

        /**
         * @brief Constructs a Slider instance.
         * 
         * Initializes the track and thumb, sets default styles, and configures event handlers.
         */
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

        /**
         * @brief Sets the value of the slider.
         * 
         * The value is clamped to the range [MinValue, MaxValue] and snapped to the nearest step.
         * 
         * @param value The new value to set.
         */
        void SetValue(float value)
        {
            m_Value = std::floor(value / Step) * Step;
            m_Value = Clamp(m_Value, MinValue, MaxValue);
        }

        /**
         * @brief Gets the current value of the slider.
         * 
         * @return The current value of the slider.
         */
        float GetValue() const
        {
            return m_Value;
        }

    private:
        /**
         * @brief Initializes the track of the slider.
         * 
         * Sets the default style for the track.
         */
        void InitTrack()
        {
            m_Track->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(12))
                    .WithBackground(BoxBackground::Solid(ColorRGB(56, 56, 56)))
            );
        }

        /**
         * @brief Initializes the thumb of the slider.
         * 
         * Sets the default style for the thumb and configures its event handlers.
         */
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

        /**
         * @brief Synchronizes the thumb position with the mouse position.
         * 
         * Updates the slider value based on the mouse position relative to the track.
         */
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

        /**
         * @brief Refreshes the thumb's position based on the current slider value.
         * 
         * Updates the thumb's position to reflect the current value of the slider.
         */
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