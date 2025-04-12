#pragma once

/**
 * @file Keyboard.h
 * @brief Provides the `Keyboard` class for managing keyboard modifiers and keys.
 * 
 * This file defines the `Keyboard` class, which allows enabling, disabling, 
 * and checking the state of keyboard modifiers (Shift, Control, Alt).
 */

namespace yap
{
    /**
     * @typedef KeyboardKey
     * @brief Represents a keyboard key as an integer.
     */
    using KeyboardKey = int;

    /**
     * @enum KeyboardModifier
     * @brief Enum representing keyboard modifier keys.
     * 
     * This enum defines the following modifiers:
     * - `Shift`: Represents the Shift key.
     * - `Control`: Represents the Control key.
     * - `Alt`: Represents the Alt key.
     */
    enum class KeyboardModifier
    {
        Shift = 0x0,
        Control = 0x1,
        Alt = 0x2
    };

    /**
     * @class Keyboard
     * @brief Manages the state of keyboard modifiers.
     * 
     * The `Keyboard` class provides methods to enable, disable, and check the 
     * state of keyboard modifiers.
     */
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