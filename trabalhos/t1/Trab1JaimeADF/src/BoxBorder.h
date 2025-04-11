#pragma once

#include "Color.h"

namespace yap
{
    enum class BoxBorderKind
    {
        None,
        Solid
    };

    class BoxBorder
    {
    private:
        BoxBorderKind m_Kind;
        ColorRGB m_Color;
        float m_Width;

    public:
        BoxBorder()
            : m_Kind(BoxBorderKind::None), m_Color(), m_Width(0.0f)
        {
        }

        bool IsNone() const
        {
            return m_Kind == BoxBorderKind::None;
        }

        bool IsSolid() const
        {
            return m_Kind == BoxBorderKind::Solid;
        }

        BoxBorderKind GetKind() const
        {
            return m_Kind;
        }

        const ColorRGB &GetColor() const
        {
            return m_Color;
        }

        float GetWidth() const
        {
            return m_Width;
        }

        static BoxBorder Solid(const ColorRGB &color, float width = 1.0f)
        {
            return BoxBorder(BoxBorderKind::Solid, color, width);
        }
    
    private:
        BoxBorder(BoxBorderKind kind, const ColorRGB &color, float width)
            : m_Kind(kind), m_Color(color), m_Width(width)
        {
        }
    };
}