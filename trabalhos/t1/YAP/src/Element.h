#pragma once

#include <functional>

#include "Axis.h"
#include "SizingRule.h"
#include "PositioningRule.h"
#include "ColorRGB.h"
#include "Vec2.h"

#include "Mouse.h"
#include "Keyboard.h"

#include "RenderingContext.h"

namespace yap
{
    class Element
    {
    private:
        bool m_Hovered = false;
        bool m_Pressed = false;
    public:
        int ZIndex = 0;

        SizingRule Size = SizingRule();
        PositioningRule Position = PositioningRule::Static();

        ColorRGB Background = ColorRGB();

        Vec2 ViewportSize = Vec2();
        Vec2 ViewportPosition = Vec2();

        std::function<void(Element&)> OnPress = nullptr;
        std::function<void(Element&)> OnRelease = nullptr;
        std::function<void(Element&, Vec2)> OnMouseMove = nullptr;
        std::function<void(Element&)> OnMouseEnter = nullptr;
        std::function<void(Element&)> OnMouseLeave = nullptr;

        virtual void ProcessMouseMove(Mouse &mouse) 
        {
            if (Intersects(mouse.Position))
            {
                if (OnMouseMove)
                {
                    OnMouseMove(*this, mouse.Position - ViewportPosition);
                }

                if (!m_Hovered)
                {
                    m_Hovered = true;

                    if (OnMouseEnter)
                    {
                        OnMouseEnter(*this);
                    }
                }
            }
            else
            {
                if (m_Hovered)
                {
                    m_Hovered = false;

                    if (OnMouseLeave)
                    {
                        OnMouseLeave(*this);
                    }
                }
            }
        }

        virtual void ProcessMouseDown(Mouse &mouse, MouseButton button) 
        {
            if (m_Hovered)
            {
                m_Pressed = true;

                if (OnPress)
                {
                    OnPress(*this);
                }
            }
        }

        virtual void ProcessMouseUp(Mouse &mouse, MouseButton button)
        {
            if (m_Pressed)
            {
                m_Pressed = false;

                if (OnRelease)
                {
                    OnRelease(*this);
                }
            }
        }

        virtual void ProcessMouseScroll(Mouse &mouse, MouseScrollDirection direction) {}
        virtual void ProcessKeyboardDown(Keyboard &keyboard, KeyboardKey key) {}
        virtual void ProcessKeyboardUp(Keyboard &keyboard, KeyboardKey key) {}

        virtual void ComputeIndependentDimensions()
        {
            if (Size.Width.IsFixed())
            {
                ViewportSize.X = Size.Width.GetValue();
            }

            if (Size.Height.IsFixed())
            {
                ViewportSize.Y = Size.Height.GetValue();
            }
        }

        virtual void ComputeResponsiveDimensions()
        {
        }

        virtual void ComputePosition()
        {
            if (Position.IsAbsolute())
            {
                ViewportPosition = Position.GetOffset();
            }
        }

        virtual void Animate() {}

        virtual void Draw(RenderingContext& context)
        {
            context.Color(Background);
            context.FillRectangle(ViewportPosition.X, ViewportPosition.Y, ViewportSize.X, ViewportSize.Y);
        }

        bool Intersects(const Vec2& point) const
        {
            return (point.X >= ViewportPosition.X && point.X <= ViewportPosition.X + ViewportSize.X &&
                    point.Y >= ViewportPosition.Y && point.Y <= ViewportPosition.Y + ViewportSize.Y);
        }

        bool IsHovered() const { 
            return m_Hovered; 
        }

        bool IsPressed() const {
            return m_Pressed;
        }
    };
}