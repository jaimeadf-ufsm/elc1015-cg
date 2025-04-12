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
        ColorRGBA m_GlobalColor;

    public:
        ColorPalette(const ColorRGBA& color = ColorRGBA(255, 255, 255, 255))
            : m_GlobalColor(color)
        {
        }

        void SetGlobalColor(const ColorRGBA& color)
        {
            m_GlobalColor = color;
        }

        const ColorRGBA& GetGlobalColor() const
        {
            return m_GlobalColor;
        }
    };
}