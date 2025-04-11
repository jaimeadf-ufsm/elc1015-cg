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
        Shift = 0x0,   ///< Shift key modifier.
        Control = 0x1, ///< Control key modifier.
        Alt = 0x2      ///< Alt key modifier.
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
        int m_Modifiers = 0; ///< Stores the current state of modifiers as a bitmask.

    public:
        /**
         * @brief Enables a keyboard modifier.
         * 
         * Sets the specified modifier in the internal bitmask to enabled.
         * 
         * @param modifier The modifier to enable (e.g., `KeyboardModifier::Shift`).
         */
        void EnableModifier(KeyboardModifier modifier)
        {
            m_Modifiers |= static_cast<int>(modifier);
        }

        /**
         * @brief Disables a keyboard modifier.
         * 
         * Clears the specified modifier in the internal bitmask to disabled.
         * 
         * @param modifier The modifier to disable (e.g., `KeyboardModifier::Control`).
         */
        void DisableModifier(KeyboardModifier modifier)
        {
            m_Modifiers &= ~static_cast<int>(modifier);
        }

        /**
         * @brief Checks if a keyboard modifier is enabled.
         * 
         * Determines whether the specified modifier is currently enabled in the 
         * internal bitmask.
         * 
         * @param modifier The modifier to check (e.g., `KeyboardModifier::Alt`).
         * @return `true` if the modifier is enabled, `false` otherwise.
         */
        bool IsModifierEnabled(KeyboardModifier modifier) const
        {
            return (m_Modifiers & static_cast<int>(modifier)) != 0;
        }
    };
}