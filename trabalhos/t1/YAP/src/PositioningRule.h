#pragma once

namespace yap
{
    enum class PositioningMode
    {
        Static,
        Float
    };

    class PositioningRule
    {
    public:
        PositioningMode m_Kind;
        int m_X;
        int m_Y;

    public:

        PositioningRule() : m_Kind(PositioningMode::Static), m_X(0), m_Y(0) {}

        PositioningMode GetMode() const
        {
            return m_Kind;
        }

        int GetX() const
        {
            return m_X;
        }

        int GetY() const
        {
            return m_Y;
        }

        static PositioningRule Static()
        {
            return PositioningRule(PositioningMode::Static, 0, 0);
        }

        static PositioningRule Float(int x, int y)
        {
            return PositioningRule(PositioningMode::Float, x, y);
        }

    private:
        PositioningRule(PositioningMode kind, int x, int y) : m_Kind(kind), m_X(x), m_Y(y) {}
    };
}