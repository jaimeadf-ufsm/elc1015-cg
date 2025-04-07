#pragma once

#include <functional>
#include <unordered_set>
#include <string>

#include "Vec2.h"
#include "StyleSheet.h"

#include "Mouse.h"
#include "Keyboard.h"

#include "RenderingContext.h"

namespace yap
{
    class Screen;

    class Element
    {
    private:
        bool m_Focused = false;
        bool m_Hovered = false;
        bool m_Pressed = false;

        std::shared_ptr<Screen> m_Screen;

        std::vector<std::pair<std::string, StyleSheet>> m_Styles;
        std::unordered_set<std::string> m_Traits;

    public:
        Vec2 Size = Vec2();
        Vec2 Position = Vec2();

        ComputedStyleSheet ComputedStyle;

        std::function<void(Element&)> OnMount = nullptr;
        std::function<void(Element&)> OnUnmount = nullptr;
        std::function<void(Element&)> OnAnimate = nullptr;
        std::function<void(Element&)> OnMouseMove = nullptr;
        std::function<void(Element&)> OnMouseEnter = nullptr;
        std::function<void(Element&)> OnMouseLeave = nullptr;
        std::function<void(Element&)> OnMousePress = nullptr;
        std::function<void(Element&)> OnMouseRelease = nullptr;
        std::function<void(Element&)> OnMouseDrag = nullptr;
        std::function<void(Element&)> OnMouseClick = nullptr;

        virtual void ProcessMouseMove(Mouse &mouse) 
        {
            if (Intersects(mouse.Position))
            {
                if (!m_Hovered)
                {
                    m_Hovered = true;
                    HandleMouseEnter(mouse);
                }

                HandleMouseMove(mouse);
            }
            else
            {
                if (m_Pressed)
                {
                    HandleMouseMove(mouse);
                }

                if (m_Hovered)
                {
                    m_Hovered = false;
                    HandleMouseLeave(mouse);
                }
            }
        }

        virtual void ProcessMouseDown(Mouse &mouse, MouseButton button) 
        {
            if (m_Hovered)
            {
                m_Focused = true;

                if (button == MouseButton::Left)
                {
                    m_Pressed = true;
                    HandleMousePress(mouse);
                }
            }
            else
            {
                m_Focused = false;
            }
        }

        virtual void ProcessMouseUp(Mouse &mouse, MouseButton button)
        {
            if (m_Pressed && button == MouseButton::Left)
            {
                m_Pressed = false;
                HandleMouseRelease(mouse);

                if (Intersects(mouse.Position))
                {
                    HandleMouseClick(mouse);
                }
            }
        }

        virtual void ProcessMouseScroll(Mouse &mouse, MouseScrollDirection direction) {}
        virtual void ProcessKeyboardDown(Keyboard &keyboard, KeyboardKey key) {}
        virtual void ProcessKeyboardUp(Keyboard &keyboard, KeyboardKey key) {}

        virtual void Mount(const std::shared_ptr<Screen>& screen)
        {
            m_Screen = screen;

            if (OnMount)
            {
                OnMount(*this);
            }
        }

        virtual void Unmount()
        {
            if (OnUnmount)
            {
                OnUnmount(*this);
            }

            m_Screen.reset();
        }

        virtual void Animate()
        {
            if (OnAnimate)
            {
                OnAnimate(*this);
            }
        }

        virtual void ComputeStyle(const ComputedStyleSheet& parentStyle)
        {
            ComputedStyle.Reset();
            ComputedStyle.Inherit(parentStyle);

            for (const auto& entry : m_Styles)
            {
                const std::string& selector = entry.first;
                const StyleSheet& style = entry.second;

                size_t colonPosition = selector.find(':');
                std::string trait = selector.substr(0, colonPosition);
                std::string state;

                if (colonPosition != std::string::npos)
                {
                    state = selector.substr(colonPosition + 1);
                }

                if (!HasTrait(trait))
                {
                    continue;
                }

                if (state == "")
                {
                    ComputedStyle.Override(style);
                }
                else if (state == "hover" && m_Hovered)
                {
                    ComputedStyle.Override(style);
                }
                else if (state == "active" && m_Pressed)
                {
                    ComputedStyle.Override(style);
                }
                else if (state == "focus" && m_Focused)
                {
                    ComputedStyle.Override(style);
                }
            }
        }

        virtual void ComputeIndependentDimensions()
        {
            if (ComputedStyle.Size.Width.IsFixed())
            {
                Size.X = ComputedStyle.Size.Width.GetValue();
            }
            else
            {
                Size.X = 0;
            }

            if (ComputedStyle.Size.Height.IsFixed())
            {
                Size.Y = ComputedStyle.Size.Height.GetValue();
            }
            else
            {
                Size.Y = 0;
            }
        }

        virtual void ComputeResponsiveDimensions()
        {
        }

        virtual void ComputePosition()
        {
            if (ComputedStyle.Position.IsAbsolute())
            {
                Position = ComputedStyle.Position.GetOffset();
            }
        }

        virtual void Draw(RenderingContext& context) = 0;

        bool Intersects(const Vec2& point) const
        {
            return (
                point.X >= Position.X && point.X <= Position.X + Size.X &&
                point.Y >= Position.Y && point.Y <= Position.Y + Size.Y
            );
        }

        bool IsHovered() const
        { 
            return m_Hovered; 
        }

        bool IsPressed() const
        {
            return m_Pressed;
        }

        void ToggleTrait(const std::string& trait, bool enable = false)
        {
            if (enable)
            {
                EnableTrait(trait);
            }
            else
            {
                DisableTrait(trait);
            }
        }

        void EnableTrait(const std::string& trait)
        {
            m_Traits.insert(trait);
        }

        void DisableTrait(const std::string& trait)
        {
            m_Traits.erase(trait);
        }

        bool HasTrait(const std::string& trait) const
        {
            if (trait.empty())
            {
                return true;
            }

            return m_Traits.find(trait) != m_Traits.end();
        }

        void SetStyle(const StyleSheet& style)
        {
            SetStyle("", style);
        }
    
        void SetStyle(const std::string& target, const StyleSheet& style)
        {
            for (auto& entry : m_Styles)
            {
                if (entry.first == target)
                {
                    entry.second = style;
                    return;
                }
            }

            m_Styles.push_back(std::make_pair(target, style));
        }

        StyleSheet GetStyle(const std::string& target = "") const
        {
            for (const auto& entry : m_Styles)
            {
                const std::string& selector = entry.first;

                if (selector == target)
                {
                    return entry.second;
                }
            }

            return StyleSheet();
        }

        const std::shared_ptr<Screen>& GetScreen() const
        {
            return m_Screen;
        }
    
    protected:
        virtual void HandleMouseMove(Mouse &mouse)
        {
            if (OnMouseMove)
            {
                OnMouseMove(*this);
            }
        }

        virtual void HandleMouseEnter(Mouse &mouse)
        {
            if (OnMouseEnter)
            {
                OnMouseEnter(*this);
            }
        }

        virtual void HandleMouseLeave(Mouse &mouse)
        {
            if (OnMouseLeave)
            {
                OnMouseLeave(*this);
            }
        }

        virtual void HandleMousePress(Mouse &mouse)
        {
            if (OnMousePress)
            {
                OnMousePress(*this);
            }
        }

        virtual void HandleMouseRelease(Mouse &mouse)
        {
            if (OnMouseRelease)
            {
                OnMouseRelease(*this);
            }
        }

        virtual void HandleMouseClick(Mouse &mouse)
        {
            if (OnMouseClick)
            {
                OnMouseClick(*this);
            }
        }
    };
}