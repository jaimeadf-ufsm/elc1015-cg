#include <memory>

#include "Element.h"
#include "Box.h"
#include "Mouse.h"
#include "Keyboard.h"

namespace yap
{
    class Screen
    {
    private:
        Mouse m_Mouse;
        Keyboard m_Keyboard;

        Box Root;

    public:
        void Resize(float width, float height)
        {
            Root.Size.Width = AxisSizingRule::Fixed(width);
            Root.Size.Height = AxisSizingRule::Fixed(height);
        }

        void ProcessMouseMove(float x, float y)
        {
            m_Mouse.Position.X = x;
            m_Mouse.Position.Y = y;
            Root.ProcessMouseMove(m_Mouse);
        }

        void ProcessMouseUp(MouseButton button)
        {
            Root.ProcessMouseUp(m_Mouse, button);
        }

        void ProcessMouseDown(MouseButton button)
        {
            Root.ProcessMouseDown(m_Mouse, button);
        }

        void ProcessMouseScroll(MouseScrollDirection direction)
        {
            Root.ProcessMouseScroll(m_Mouse, direction);
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

            Root.ProcessKeyboardDown(m_Keyboard, key);
        }

        void ProcessKeyboardUp(KeyboardKey key)
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

            Root.ProcessKeyboardUp(m_Keyboard, key);
        }

        void Render(RenderingContext& context)
        {
            Root.ComputeIndependentDimensions();
            Root.ComputeResponsiveDimensions();
            Root.ComputePosition();
            Root.Animate();
            Root.Draw(context);
        }

        Box &GetRoot()
        {
            return Root;
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