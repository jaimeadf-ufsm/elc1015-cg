#pragma once

#include <memory>

#include "Element.h"
#include "Box.h"
#include "Mouse.h"
#include "Keyboard.h"

/**
 * @file Screen.h
 * @brief Defines the `Screen` class, which serves as the main interface for managing user interactions,
 * rendering, and event processing in a graphical application.
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
        Mouse m_Mouse;
        Keyboard m_Keyboard;

        std::vector<std::function<void()>> m_CurrentFrameCallbacks;
        std::vector<std::function<void()>> m_NextFrameCallbacks;

    public:
        std::shared_ptr<Box> Root;

        Screen() : Root(std::make_shared<Box>())
        {
        }

        void Init()
        {
            Root->Mount(shared_from_this());
        }

        void Resize(float width, float height)
        {
            StyleSheet style = Root->GetStyle();
            style.Size = SizingRule(AxisSizingRule::Fixed(width), AxisSizingRule::Fixed(height));

            Root->SetStyle(style);
        }

        void ProcessMouseMove(float x, float y)
        {
            m_Mouse.Position.X = x;
            m_Mouse.Position.Y = y;
            Root->ProcessMouseMove(m_Mouse);
        }

        void ProcessMouseUp(MouseButton button)
        {
            Root->ProcessMouseUp(m_Mouse, button);
        }

        void ProcessMouseDown(MouseButton button)
        {
            Root->ProcessMouseDown(m_Mouse, button);
        }

        void ProcessMouseScroll(MouseScrollDirection direction)
        {
            Root->ProcessMouseScroll(m_Mouse, direction);
        }

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

        void ExecuteNextFrame(const std::function<void()>& callback)
        {
            m_NextFrameCallbacks.emplace_back(callback);
        }

        const Mouse& GetMouse() const
        {
            return m_Mouse;
        }

        const Keyboard& GetKeyboard() const
        {
            return m_Keyboard;
        }
    };
}