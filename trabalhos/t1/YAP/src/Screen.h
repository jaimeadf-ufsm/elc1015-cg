#pragma once

#include <memory>

#include "Element.h"
#include "Box.h"
#include "Mouse.h"
#include "Keyboard.h"

namespace yap
{
    class Screen : public std::enable_shared_from_this<Screen>
    {
    private:
        Mouse m_Mouse;
        Keyboard m_Keyboard;

    public:
        std::shared_ptr<Box> Root;

        Screen() : Root(std::make_shared<Box>())
        {
            Root->SetStyle(StyleSheet().WithZIndex(0));
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
            Root->Animate();
            Root->ComputeStyle(ComputedStyleSheet());
            Root->ComputeIndependentDimensions();
            Root->ComputeResponsiveDimensions();
            Root->ComputePosition();
            Root->Draw(context);
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