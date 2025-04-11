#pragma once

#include <functional>
#include <unordered_set>
#include <string>

#include "Vec2.h"
#include "StyleSheet.h"

#include "Mouse.h"
#include "Keyboard.h"

#include "RenderingContext.h"

/**
 * @file Element.h
 * @brief Defines the Element class, which serves as a base class for UI components in the rendering system.
 */

namespace yap
{
    class Screen;

    /**
     * @class Element
     * @brief Represents a UI element that can handle user interactions, animations, and rendering.
     */
    class Element
    {
    private:
        bool m_Focused = false; ///< Indicates whether the element is focused.
        bool m_Hovered = false; ///< Indicates whether the mouse is hovering over the element.
        bool m_Pressed = false; ///< Indicates whether the element is being pressed.

        std::shared_ptr<Screen> m_Screen; ///< The screen to which this element belongs.

        std::vector<std::pair<std::string, StyleSheet>> m_Styles; ///< List of styles applied to the element.
        std::unordered_set<std::string> m_Traits; ///< Set of traits associated with the element.

    public:
        Vec2 Size = Vec2(); ///< The size of the element.
        Vec2 Position = Vec2(); ///< The position of the element.

        ComputedStyleSheet ComputedStyle; ///< The computed style of the element.

        // Event handlers
        std::function<void(Element&)> OnMount = nullptr; ///< Called when the element is mounted.
        std::function<void(Element&)> OnUnmount = nullptr; ///< Called when the element is unmounted.
        std::function<void(Element&)> OnAnimate = nullptr; ///< Called during the animation phase.
        std::function<void(Element&)> OnFocus = nullptr; ///< Called when the element gains focus.
        std::function<void(Element&)> OnUnfocus = nullptr; ///< Called when the element loses focus.
        std::function<void(Element&)> OnMouseMove = nullptr; ///< Called when the mouse moves over the element.
        std::function<void(Element&)> OnMouseEnter = nullptr; ///< Called when the mouse enters the element.
        std::function<void(Element&)> OnMouseLeave = nullptr; ///< Called when the mouse leaves the element.
        std::function<void(Element&)> OnMousePress = nullptr; ///< Called when the mouse presses the element.
        std::function<void(Element&)> OnMouseRelease = nullptr; ///< Called when the mouse releases the element.
        std::function<void(Element&, KeyboardKey)> OnKeyboardDown = nullptr; ///< Called when a keyboard key is pressed.
        std::function<void(Element&, KeyboardKey)> OnKeyboardUp = nullptr; ///< Called when a keyboard key is released.

        /**
         * @brief Processes mouse movement events.
         * @param mouse The mouse object containing the current state.
         */
        virtual void ProcessMouseMove(Mouse &mouse)
        {
            if (ComputedStyle.Events == PointerEvents::None)
            {
                return;
            }

            if (Intersects(mouse.Position))
            {
                if (!m_Hovered)
                {
                    m_Hovered = true;

                    if (OnMouseEnter)
                    {
                        OnMouseEnter(*this);
                    }
                }

                if (OnMouseMove)
                {
                    OnMouseMove(*this);
                }
            }
            else
            {
                if (m_Pressed)
                {
                    if (OnMouseMove)
                    {
                        OnMouseMove(*this);
                    }
                }

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

        /**
         * @brief Processes mouse button down events.
         * @param mouse The mouse object containing the current state.
         * @param button The mouse button that was pressed.
         */
        virtual void ProcessMouseDown(Mouse &mouse, MouseButton button)
        {
            if (ComputedStyle.Events == PointerEvents::None)
            {
                return;
            }

            if (m_Hovered)
            {
                if (!m_Focused)
                {
                    Focus();
                }

                if (button == MouseButton::Left)
                {
                    m_Pressed = true;

                    if (OnMousePress)
                    {
                        OnMousePress(*this);
                    }
                }
            }
            else
            {
                Unfocus();
            }
        }

        /**
         * @brief Processes mouse button up events.
         * @param mouse The mouse object containing the current state.
         * @param button The mouse button that was released.
         */
        virtual void ProcessMouseUp(Mouse &mouse, MouseButton button)
        {
            if (ComputedStyle.Events == PointerEvents::None)
            {
                return;
            }

            if (m_Pressed && button == MouseButton::Left)
            {
                m_Pressed = false;
                
                if (OnMouseRelease)
                {
                    OnMouseRelease(*this);
                }
            }
        }

        /**
         * @brief Processes mouse scroll events.
         * @param mouse The mouse object containing the current state.
         * @param direction The direction of the scroll.
         */
        virtual void ProcessMouseScroll(Mouse &mouse, MouseScrollDirection direction) {}

        /**
         * @brief Processes keyboard key down events.
         * @param keyboard The keyboard object containing the current state.
         * @param key The key that was pressed.
         */
        virtual void ProcessKeyboardDown(Keyboard &keyboard, KeyboardKey key)
        {
            if (m_Focused)
            {
                if (OnKeyboardDown)
                {
                    OnKeyboardDown(*this, key);
                }
            }
        }

        /**
         * @brief Processes keyboard key up events.
         * @param keyboard The keyboard object containing the current state.
         * @param key The key that was released.
         */
        virtual void ProcessKeyboardUp(Keyboard &keyboard, KeyboardKey key)
        {
            if (m_Focused)
            {
                if (OnKeyboardUp)
                {
                    OnKeyboardUp(*this, key);
                }
            }
        }

        /**
         * @brief Mounts the element to a screen.
         * @param screen The screen to which the element is mounted.
         */
        virtual void Mount(const std::shared_ptr<Screen>& screen)
        {
            m_Screen = screen;

            if (OnMount)
            {
                OnMount(*this);
            }
        }

        /**
         * @brief Unmounts the element from its screen.
         */
        virtual void Unmount()
        {
            if (OnUnmount)
            {
                OnUnmount(*this);
            }

            m_Screen.reset();
        }

        /**
         * @brief Animates the element.
         */
        virtual void Animate()
        {
            if (OnAnimate)
            {
                OnAnimate(*this);
            }
        }

        /**
         * @brief Computes the style of the element based on its parent style.
         * @param parentStyle The computed style of the parent element.
         */
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
            
            if (ComputedStyle.Events == PointerEvents::None)
            {
                if (m_Hovered)
                {
                    m_Hovered = false;

                    if (OnMouseLeave)
                    {
                        OnMouseLeave(*this);
                    }
                }

                if (m_Pressed)
                {
                    m_Pressed = false;

                    if (OnMouseRelease)
                    {
                        OnMouseRelease(*this);
                    }
                }
            }
        }

        /**
         * @brief Computes the independent dimensions of the element.
         */
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

        /**
         * @brief Computes the responsive dimensions of the element.
         */
        virtual void ComputeResponsiveDimensions()
        {
        }

        /**
         * @brief Computes the position of the element.
         */
        virtual void ComputePosition()
        {
            if (ComputedStyle.Position.IsAbsolute())
            {
                Position = ComputedStyle.Position.GetOffset();
            }
        }

        /**
         * @brief Draws the element on the rendering context.
         * @param context The rendering context.
         */
        virtual void Draw(RenderingContext& context) = 0;

        /**
         * @brief Checks if a point intersects with the element.
         * @param point The point to check.
         * @return True if the point intersects with the element, false otherwise.
         */
        bool Intersects(const Vec2& point) const
        {
            return (
                point.X >= Position.X && point.X <= Position.X + Size.X &&
                point.Y >= Position.Y && point.Y <= Position.Y + Size.Y
            );
        }

        /**
         * @brief Focuses the element.
         */
        void Focus()
        {
            if (!m_Focused)
            {
                m_Focused = true;

                if (OnFocus)
                {
                    OnFocus(*this);
                }
            }
        }

        /**
         * @brief Unfocuses the element.
         */
        void Unfocus()
        {
            if (m_Focused)
            {
                m_Focused = false;

                if (OnUnfocus)
                {
                    OnUnfocus(*this);
                }
            }
        }

        /**
         * @brief Checks if the element is hovered.
         * @return True if the element is hovered, false otherwise.
         */
        bool IsHovered() const
        { 
            return m_Hovered; 
        }

        /**
         * @brief Checks if the element is pressed.
         * @return True if the element is pressed, false otherwise.
         */
        bool IsPressed() const
        {
            return m_Pressed;
        }

        /**
         * @brief Checks if the element is focused.
         * @return True if the element is focused, false otherwise.
         */
        bool IsFocused() const
        {
            return m_Focused;
        }

        /**
         * @brief Toggles a trait for the element.
         * @param trait The trait to toggle.
         * @param enable Whether to enable or disable the trait.
         */
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

        /**
         * @brief Enables a trait for the element.
         * @param trait The trait to enable.
         */
        void EnableTrait(const std::string& trait)
        {
            m_Traits.insert(trait);
        }

        /**
         * @brief Disables a trait for the element.
         * @param trait The trait to disable.
         */
        void DisableTrait(const std::string& trait)
        {
            m_Traits.erase(trait);
        }

        /**
         * @brief Checks if the element has a specific trait.
         * @param trait The trait to check.
         * @return True if the element has the trait, false otherwise.
         */
        bool HasTrait(const std::string& trait) const
        {
            if (trait.empty())
            {
                return true;
            }

            return m_Traits.find(trait) != m_Traits.end();
        }

        /**
         * @brief Sets a style for the element.
         * @param style The style to set.
         */
        void SetStyle(const StyleSheet& style)
        {
            SetStyle("", style);
        }
    
        /**
         * @brief Sets a style for a specific target of the element.
         * @param target The target selector.
         * @param style The style to set.
         */
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

        /**
         * @brief Gets the style for a specific target of the element.
         * @param target The target selector.
         * @return The style associated with the target.
         */
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

        /**
         * @brief Gets the screen to which the element belongs.
         * @return A shared pointer to the screen.
         */
        const std::shared_ptr<Screen>& GetScreen() const
        {
            return m_Screen;
        }
    };
}