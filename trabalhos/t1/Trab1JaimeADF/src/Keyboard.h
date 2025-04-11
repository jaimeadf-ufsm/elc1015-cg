#pragma once

namespace yap
{
    using KeyboardKey = int;

    enum class KeyboardModifier
    {
        Shift = 0x0,
        Control = 0x1,
        Alt = 0x2
    };

    class Keyboard
    {
    private:
        int m_Modifiers = 0;

    public:
        void EnableModifier(KeyboardModifier modifier)
        {
            m_Modifiers |= static_cast<int>(modifier);
        }

        void DisableModifier(KeyboardModifier modifier)
        {
            m_Modifiers &= ~static_cast<int>(modifier);
        }

        bool IsModifierEnabled(KeyboardModifier modifier) const
        {
            return (m_Modifiers & static_cast<int>(modifier)) != 0;
        }
    };
}