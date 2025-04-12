#pragma once

#include "Color.h"

namespace yap
{
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