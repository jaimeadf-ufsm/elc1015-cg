#pragma once

namespace yap
{
    enum class SizingMode
    {
        Fixed,
        Fit,
        Fill
    };

    class SizingRule
    {
    private:
        SizingMode m_Mode;
        int m_Value;

    public:
        SizingRule() : m_Mode(SizingMode::Fit), m_Value(0) {}
        SizingRule(const SizingRule& other) : m_Mode(other.m_Mode), m_Value(other.m_Value) {}

        bool IsIndependent() const
        {
            return m_Mode == SizingMode::Fixed || m_Mode == SizingMode::Fit;
        }

        bool IsFixed() const
        {
            return m_Mode == SizingMode::Fixed;
        }

        bool IsFit() const
        {
            return m_Mode == SizingMode::Fit;
        }

        bool IsFill() const
        {
            return m_Mode == SizingMode::Fill;
        }

        SizingMode GetMode() const
        {
            return m_Mode;
        }

        int GetValue() const
        {
            return m_Value;
        }

        static SizingRule Fit()
        {
            return SizingRule(SizingMode::Fit, 0);
        }

        static SizingRule Fixed(int value)
        {
            return SizingRule(SizingMode::Fixed, value);
        }

        static SizingRule Fill()
        {
            return SizingRule(SizingMode::Fill, 0);
        }

    private:
        SizingRule(SizingMode kind, int value) : m_Mode(kind), m_Value(value) {}
    };
}