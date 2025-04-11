#pragma once

// Summary:
// This file defines the `AxisSizingRule` and `SizingRule` classes, which are used to manage sizing rules
// for UI elements along specific axes. The `AxisSizingRule` class encapsulates the sizing mode and value
// for a single axis, while the `SizingRule` class combines rules for both width and height.

namespace yap
{
    // Enum: AxisSizingMode
    // Represents the sizing mode for an axis.
    enum class AxisSizingMode
    {
        Fixed, // The size is fixed to a specific value.
        Fit,   // The size is determined based on the content.
        Fill   // The size fills the available space.
    };

    // Class: AxisSizingRule
    // Represents a sizing rule for a single axis, including the sizing mode and an optional value.
    class AxisSizingRule
    {
    private:
        AxisSizingMode m_Mode; // The sizing mode for the axis.
        int m_Value;           // The value associated with the sizing mode (used for Fixed mode).

    public:
        // Constructor: AxisSizingRule
        // Initializes the rule with the default mode (Fit) and value (0).
        AxisSizingRule() : m_Mode(AxisSizingMode::Fit), m_Value(0) {}

        // Copy Constructor: AxisSizingRule
        // Creates a copy of another AxisSizingRule.
        AxisSizingRule(const AxisSizingRule& other) : m_Mode(other.m_Mode), m_Value(other.m_Value) {}

        // Method: GetMode
        // Returns the sizing mode of the rule.
        AxisSizingMode GetMode() const
        {
            return m_Mode;
        }

        // Method: GetValue
        // Returns the value associated with the sizing rule.
        int GetValue() const
        {
            return m_Value;
        }

        // Method: IsIndependent
        // Checks if the sizing mode is either Fixed or Fit.
        bool IsIndependent() const
        {
            return m_Mode == AxisSizingMode::Fixed || m_Mode == AxisSizingMode::Fit;
        }

        // Method: IsFixed
        // Checks if the sizing mode is Fixed.
        bool IsFixed() const
        {
            return m_Mode == AxisSizingMode::Fixed;
        }

        // Method: IsFit
        // Checks if the sizing mode is Fit.
        bool IsFit() const
        {
            return m_Mode == AxisSizingMode::Fit;
        }

        // Method: IsFill
        // Checks if the sizing mode is Fill.
        bool IsFill() const
        {
            return m_Mode == AxisSizingMode::Fill;
        }

        // Static Method: Fit
        // Creates an AxisSizingRule with the Fit mode.
        static AxisSizingRule Fit()
        {
            return AxisSizingRule(AxisSizingMode::Fit, 0);
        }

        // Static Method: Fixed
        // Creates an AxisSizingRule with the Fixed mode and a specified value.
        static AxisSizingRule Fixed(int value)
        {
            return AxisSizingRule(AxisSizingMode::Fixed, value);
        }

        // Static Method: Fill
        // Creates an AxisSizingRule with the Fill mode.
        static AxisSizingRule Fill()
        {
            return AxisSizingRule(AxisSizingMode::Fill, 0);
        }

    private:
        // Constructor: AxisSizingRule
        // Initializes the rule with a specific mode and value.
        AxisSizingRule(AxisSizingMode kind, int value) : m_Mode(kind), m_Value(value) {}
    };

    // Struct: SizingRule
    // Represents sizing rules for both width and height.
    struct SizingRule
    {
        AxisSizingRule Width;  // The sizing rule for the width.
        AxisSizingRule Height; // The sizing rule for the height.

        // Constructor: SizingRule
        // Initializes both width and height with the Fit mode.
        SizingRule() : Width(AxisSizingRule::Fit()), Height(AxisSizingRule::Fit()) {}

        // Constructor: SizingRule
        // Initializes the width and height with specific sizing rules.
        SizingRule(AxisSizingRule width, AxisSizingRule height) : Width(width), Height(height) {}

        // Method: SetSizeAlongAxis
        // Sets the sizing rule for a specific axis.
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

        // Method: GetSizeAlongAxis
        // Retrieves the sizing rule for a specific axis.
        AxisSizingRule GetSizeAlongAxis(Axis axis)
        {
            return (axis == Axis::X ? Width : Height);
        }
    };
}