#pragma once

#include "Color.h"

/**
 * @file ColorPalette.h
 * @brief Defines the ColorPalette class, which manages a global color for rendering or other purposes.
 */

namespace yap
{
    /**
     * @class ColorPalette
     * @brief A class to manage a global RGBA color.
     * 
     * This class provides functionality to set and retrieve a global color, 
     * which can be used in rendering or other graphical operations.
     */
    class ColorPalette 
    {
    private:
        ColorRGBA m_GlobalColor; ///< The global RGBA color managed by this palette.

    public:
        /**
         * @brief Constructs a ColorPalette with an optional initial color.
         * 
         * @param color The initial global color. Defaults to white (255, 255, 255, 255).
         */
        ColorPalette(const ColorRGBA& color = ColorRGBA(255, 255, 255, 255))
            : m_GlobalColor(color)
        {
        }

        /**
         * @brief Sets the global color.
         * 
         * @param color The new global color to set.
         */
        void SetGlobalColor(const ColorRGBA& color)
        {
            m_GlobalColor = color;
        }

        /**
         * @brief Retrieves the current global color.
         * 
         * @return A constant reference to the current global color.
         */
        const ColorRGBA& GetGlobalColor() const
        {
            return m_GlobalColor;
        }
    };
}