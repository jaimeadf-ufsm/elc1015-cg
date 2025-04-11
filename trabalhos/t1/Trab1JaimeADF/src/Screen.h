#pragma once

#include <memory>

#include "Element.h"
#include "Box.h"
#include "Mouse.h"
#include "Keyboard.h"

/**
 * @file Screen.h
 * @brief Defines the `Screen` class, which serves as the main interface for managing user interactions,
 *        rendering, and event processing in a graphical application.
 */

namespace yap
{
    /**
     * @class Screen
     * @brief Represents the main interface for managing graphical elements, user input, and rendering.
     * 
     * The `Screen` class is responsible for handling user input (mouse and keyboard), managing the root
     * graphical element (`Box`), and rendering the graphical interface. It also provides mechanisms for
     * scheduling callbacks to be executed in the next frame.
     */
    class Screen : public std::enable_shared_from_this<Screen>
    {
    private:
        Mouse m_Mouse; ///< Stores the current state of the mouse.
        Keyboard m_Keyboard; ///< Stores the current state of the keyboard.

        std::vector<std::function<void()>> m_CurrentFrameCallbacks; ///< Callbacks to execute in the current frame.
        std::vector<std::function<void()>> m_NextFrameCallbacks; ///< Callbacks to execute in the next frame.

    public:
        std::shared_ptr<Box> Root; ///< The root graphical element of the screen.

        /**
         * @brief Constructs a `Screen` object and initializes the root element.
         */
        Screen() : Root(std::make_shared<Box>())
        {
        }

        /**
         * @brief Initializes the screen by mounting the root element.
         */
        void Init()
        {
            Root->Mount(shared_from_this());
        }

        /**
         * @brief Resizes the screen by updating the root element's size.
         * 
         * @param width The new width of the screen.
         * @param height The new height of the screen.
         */
        void Resize(float width, float height)
        {
            StyleSheet style = Root->GetStyle();
            style.Size = SizingRule(AxisSizingRule::Fixed(width), AxisSizingRule::Fixed(height));

            Root->SetStyle(style);
        }

        /**
         * @brief Processes a mouse move event.
         * 
         * @param x The new X position of the mouse.
         * @param y The new Y position of the mouse.
         */
        void ProcessMouseMove(float x, float y)
        {
            m_Mouse.Position.X = x;
            m_Mouse.Position.Y = y;
            Root->ProcessMouseMove(m_Mouse);
        }

        /**
         * @brief Processes a mouse button release event.
         * 
         * @param button The mouse button that was released.
         */
        void ProcessMouseUp(MouseButton button)
        {
            Root->ProcessMouseUp(m_Mouse, button);
        }

        /**
         * @brief Processes a mouse button press event.
         * 
         * @param button The mouse button that was pressed.
         */
        void ProcessMouseDown(MouseButton button)
        {
            Root->ProcessMouseDown(m_Mouse, button);
        }

        /**
         * @brief Processes a mouse scroll event.
         * 
         * @param direction The direction of the mouse scroll.
         */
        void ProcessMouseScroll(MouseScrollDirection direction)
        {
            Root->ProcessMouseScroll(m_Mouse, direction);
        }

        /**
         * @brief Processes a keyboard key press event.
         * 
         * @param key The key that was pressed.
         */
        void ProcessKeyboardDown(KeyboardKey key)
        {
            switch (key)
            {
                case 212:
                    m_Keyboard.EnableModifier(KeyboardModifier::Shift);
                    break;
                case 214:
                    m_Keyboard.EnableModifier(KeyboardModifier::Control);
                    break;
                case 216:
                    m_Keyboard.EnableModifier(KeyboardModifier::Alt);
                    break;
            }

            Root->ProcessKeyboardDown(m_Keyboard, key);
        }

        /**
         * @brief Processes a keyboard key release event.
         * 
         * @param key The key that was released.
         */
        void ProcessKeyboardUp(KeyboardKey key)
        {
            switch (key)
            {
                case 212:
                    m_Keyboard.DisableModifier(KeyboardModifier::Shift);
                    break;
                case 214:
                    m_Keyboard.DisableModifier(KeyboardModifier::Control);
                    break;
                case 216:
                    m_Keyboard.DisableModifier(KeyboardModifier::Alt);
                    break;
            }

            Root->ProcessKeyboardUp(m_Keyboard, key);
        }

        /**
         * @brief Renders the screen by executing callbacks, animating, and drawing the root element.
         * 
         * @param context The rendering context used for drawing.
         */
        void Render(RenderingContext& context)
        {
            m_CurrentFrameCallbacks.clear();

            std::swap(m_CurrentFrameCallbacks, m_NextFrameCallbacks);

            for (const auto& callback : m_CurrentFrameCallbacks)
            {
                callback();
            }

            Root->Animate();
            Root->ComputeStyle(ComputedStyleSheet());
            Root->ComputeIndependentDimensions();
            Root->ComputeResponsiveDimensions();
            Root->ComputePosition();
            Root->Draw(context);
        }

        /**
         * @brief Schedules a callback to be executed in the next frame.
         * 
         * @param callback The callback function to execute.
         */
        void ExecuteNextFrame(const std::function<void()>& callback)
        {
            m_NextFrameCallbacks.emplace_back(callback);
        }

        /**
         * @brief Gets the current state of the mouse.
         * 
         * @return A constant reference to the `Mouse` object.
         */
        const Mouse& GetMouse() const
        {
            return m_Mouse;
        }

        /**
         * @brief Gets the current state of the keyboard.
         * 
         * @return A constant reference to the `Keyboard` object.
         */
        const Keyboard& GetKeyboard() const
        {
            return m_Keyboard;
        }
    };
}