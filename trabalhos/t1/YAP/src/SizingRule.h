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
    public:
        const AxisSizingMode Mode;
        const int Value;

        AxisSizingRule() : Mode(AxisSizingMode::Fit), Value(0) {}
        AxisSizingRule(AxisSizingMode kind, int value) : Mode(kind), Value(value) {}
        AxisSizingRule(const AxisSizingRule& other) : Mode(other.Mode), Value(other.Value) {}

        static const AxisSizingRule Fit()
        {
            return AxisSizingRule(AxisSizingMode::Fit, 0);
        }

        static const AxisSizingRule Fixed(int value)
        {
            return AxisSizingRule(AxisSizingMode::Fixed, value);
        }

        static const AxisSizingRule Fill()
        {
            return AxisSizingRule(AxisSizingMode::Fill, 0);
        }

    private:
    };

    class SizingRule
    {
    public:
        const AxisSizingRule Width;
        const AxisSizingRule Height;

        SizingRule() : Width(AxisSizingRule::Fit()), Height(AxisSizingRule::Fit()) {}
        SizingRule(AxisSizingRule width, AxisSizingRule height) : Width(width), Height(height) {}
        SizingRule(const SizingRule& other) : Width(other.Width), Height(other.Height) {}
    };
}