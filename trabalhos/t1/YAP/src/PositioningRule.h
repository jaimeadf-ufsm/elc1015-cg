#pragma once

#include "Vec2.h"

namespace yap
{
    enum class PositioningMode
    {
        Static,
        Relative,
        Absolute
    };

    class PositioningRule
    {
    public:
        PositioningMode m_Mode;
        Vec2 m_Offset;

    public:

        PositioningRule() : m_Mode(PositioningMode::Static), m_Offset() {}

        PositioningMode GetMode() const
        {
            return m_Mode;
        }

        const Vec2& GetOffset() const
        {
            return m_Offset;
        }

        bool IsStatic()
        {
            return m_Mode == PositioningMode::Static;
        }

        bool IsRelative()
        {
            return m_Mode == PositioningMode::Relative;
        }

        bool IsAbsolute()
        {
            return m_Mode == PositioningMode::Absolute;
        }

        static PositioningRule Static()
        {
            return PositioningRule(PositioningMode::Static, Vec2());
        }

        static PositioningRule Relative(const Vec2& offset)
        {
            return PositioningRule(PositioningMode::Relative, offset);
        }

        static PositioningRule Absolute(const Vec2& offset)
        {
            return PositioningRule(PositioningMode::Absolute, offset);
        }
    private:
        PositioningRule(PositioningMode kind, const Vec2& offset) :
            m_Mode(kind), m_Offset(offset) {}
    };
}