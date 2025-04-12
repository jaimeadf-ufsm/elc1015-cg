#pragma once

namespace yap
{
    enum class AxisSizingMode
    {
        Fixed,
        Fit,
        Fill
    };

    class AxisSizingRule
    {
    private:
        AxisSizingMode m_Mode;
        int m_Value;

    public:
        AxisSizingRule() : m_Mode(AxisSizingMode::Fit), m_Value(0) {}
        AxisSizingRule(const AxisSizingRule& other) : m_Mode(other.m_Mode), m_Value(other.m_Value) {}

        AxisSizingMode GetMode() const
        {
            return m_Mode;
        }

        int GetValue() const
        {
            return m_Value;
        }

        bool IsIndependent() const
        {
            return m_Mode == AxisSizingMode::Fixed || m_Mode == AxisSizingMode::Fit;
        }

        bool IsFixed() const
        {
            return m_Mode == AxisSizingMode::Fixed;
        }

        bool IsFit() const
        {
            return m_Mode == AxisSizingMode::Fit;
        }

        bool IsFill() const
        {
            return m_Mode == AxisSizingMode::Fill;
        }

        static AxisSizingRule Fit()
        {
            return AxisSizingRule(AxisSizingMode::Fit, 0);
        }

        static AxisSizingRule Fixed(int value)
        {
            return AxisSizingRule(AxisSizingMode::Fixed, value);
        }

        static AxisSizingRule Fill()
        {
            return AxisSizingRule(AxisSizingMode::Fill, 0);
        }

    private:
        AxisSizingRule(AxisSizingMode kind, int value) : m_Mode(kind), m_Value(value) {}
    };

    struct SizingRule
    {
        AxisSizingRule Width;
        AxisSizingRule Height;

        SizingRule() : Width(AxisSizingRule::Fit()), Height(AxisSizingRule::Fit()) {}
        SizingRule(AxisSizingRule width, AxisSizingRule height) : Width(width), Height(height) {}

        void SetSizeAlongAxis(Axis axis, AxisSizingRule size)
        {
            if (axis == Axis::X)
            {
                Width = size;
            }
            else
            {
                Height = size;
            }
        }

        AxisSizingRule GetSizeAlongAxis(Axis axis)
        {
            return (axis == Axis::X ? Width : Height);
        }
    };
}