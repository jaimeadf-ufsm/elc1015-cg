#pragma once

// Summary:
// This file defines the `BoxBorder` class, which represents the border of a box with customizable kind, color, and width.
// It includes utility methods to check the border type and retrieve its properties.

#include "Color.h"

namespace yap
{
    /**
     * @enum BoxBorderKind
     * @brief Represents the type of a box border.
     */
    enum class BoxBorderKind
    {
        None,  ///< No border.
        Solid  ///< Solid border.
    };

    /**
     * @class BoxBorder
     * @brief Represents a box border with a specific kind, color, and width.
     */
    class BoxBorder
    {
    private:
        BoxBorderKind m_Kind; ///< The kind of the border (e.g., None, Solid).
        ColorRGB m_Color;     ///< The color of the border.
        float m_Width;        ///< The width of the border.

    public:
        /**
         * @brief Default constructor. Creates a border with no kind, default color, and zero width.
         */
        BoxBorder()
            : m_Kind(BoxBorderKind::None), m_Color(), m_Width(0.0f)
        {
        }

        /**
         * @brief Checks if the border has no kind (None).
         * @return True if the border kind is None, false otherwise.
         */
        bool IsNone() const
        {
            return m_Kind == BoxBorderKind::None;
        }

        /**
         * @brief Checks if the border is solid.
         * @return True if the border kind is Solid, false otherwise.
         */
        bool IsSolid() const
        {
            return m_Kind == BoxBorderKind::Solid;
        }

        /**
         * @brief Gets the kind of the border.
         * @return The kind of the border as a `BoxBorderKind` enum.
         */
        BoxBorderKind GetKind() const
        {
            return m_Kind;
        }

        /**
         * @brief Gets the color of the border.
         * @return A constant reference to the `ColorRGB` object representing the border color.
         */
        const ColorRGB &GetColor() const
        {
            return m_Color;
        }

        /**
         * @brief Gets the width of the border.
         * @return The width of the border as a float.
         */
        float GetWidth() const
        {
            return m_Width;
        }

        /**
         * @brief Creates a solid border with the specified color and width.
         * @param color The color of the border.
         * @param width The width of the border. Defaults to 1.0f.
         * @return A `BoxBorder` object representing a solid border.
         */
        static BoxBorder Solid(const ColorRGB &color, float width = 1.0f)
        {
            return BoxBorder(BoxBorderKind::Solid, color, width);
        }

    private:
        /**
         * @brief Private constructor for creating a border with specific properties.
         * @param kind The kind of the border.
         * @param color The color of the border.
         * @param width The width of the border.
         */
        BoxBorder(BoxBorderKind kind, const ColorRGB &color, float width)
            : m_Kind(kind), m_Color(color), m_Width(width)
        {
        }
    };
}