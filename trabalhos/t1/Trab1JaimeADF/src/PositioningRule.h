#pragma once

#include "Vec2.h"

/**
 * @file PositioningRule.h
 * @brief Defines the PositioningRule class, which provides rules for positioning elements
 *        using different modes (Static, Relative, Absolute) and offsets.
 */

namespace yap
{
    /**
     * @enum PositioningMode
     * @brief Represents the different modes of positioning.
     */
    enum class PositioningMode
    {
        Static,   ///< The position is fixed and does not depend on any external factors.
        Relative, ///< The position is relative to its parent.
        Absolute  ///< The position is absolute in a given coordinate space.
    };

    /**
     * @class PositioningRule
     * @brief Encapsulates positioning rules, including mode and offset, for elements.
     */
    class PositioningRule
    {
    public:
        PositioningMode m_Mode; ///< The positioning mode (Static, Relative, Absolute).
        Vec2 m_Offset;          ///< The offset applied to the position.

    public:
        /**
         * @brief Default constructor. Initializes the rule with Static mode and no offset.
         */
        PositioningRule() : m_Mode(PositioningMode::Static), m_Offset() {}

        /**
         * @brief Gets the current positioning mode.
         * @return The positioning mode.
         */
        PositioningMode GetMode() const
        {
            return m_Mode;
        }

        /**
         * @brief Gets the offset associated with the positioning rule.
         * @return A reference to the offset vector.
         */
        const Vec2& GetOffset() const
        {
            return m_Offset;
        }

        /**
         * @brief Checks if the positioning mode is Static.
         * @return True if the mode is Static, false otherwise.
         */
        bool IsStatic()
        {
            return m_Mode == PositioningMode::Static;
        }

        /**
         * @brief Checks if the positioning mode is Relative.
         * @return True if the mode is Relative, false otherwise.
         */
        bool IsRelative()
        {
            return m_Mode == PositioningMode::Relative;
        }

        /**
         * @brief Checks if the positioning mode is Absolute.
         * @return True if the mode is Absolute, false otherwise.
         */
        bool IsAbsolute()
        {
            return m_Mode == PositioningMode::Absolute;
        }

        /**
         * @brief Creates a PositioningRule with Static mode and no offset.
         * @return A PositioningRule object with Static mode.
         */
        static PositioningRule Static()
        {
            return PositioningRule(PositioningMode::Static, Vec2());
        }

        /**
         * @brief Creates a PositioningRule with Relative mode and a specified offset.
         * @param offset The offset to apply in Relative mode.
         * @return A PositioningRule object with Relative mode and the given offset.
         */
        static PositioningRule Relative(const Vec2& offset)
        {
            return PositioningRule(PositioningMode::Relative, offset);
        }

        /**
         * @brief Creates a PositioningRule with Absolute mode and a specified offset.
         * @param offset The offset to apply in Absolute mode.
         * @return A PositioningRule object with Absolute mode and the given offset.
         */
        static PositioningRule Absolute(const Vec2& offset)
        {
            return PositioningRule(PositioningMode::Absolute, offset);
        }

    private:
        /**
         * @brief Private constructor to initialize a PositioningRule with a specific mode and offset.
         * @param kind The positioning mode.
         * @param offset The offset to apply.
         */
        PositioningRule(PositioningMode kind, const Vec2& offset) :
            m_Mode(kind), m_Offset(offset) {}
    };
}