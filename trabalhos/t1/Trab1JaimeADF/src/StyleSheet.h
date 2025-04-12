#pragma once

#include "Option.h"
#include "PointerEvents.h"
#include "SizingRule.h"
#include "PositioningRule.h"
#include "BoxBackground.h"
#include "BoxBorder.h"
#include "BoxDirection.h"
#include "BoxAlignment.h"
#include "BoxPadding.h"

/**
 * @file StyleSheet.h
 * @brief Defines the `StyleSheet` and `ComputedStyleSheet` classes for managing UI styles.
 * 
 * The `StyleSheet` class provides a flexible way to define optional style properties for UI elements.
 * The `ComputedStyleSheet` class represents the resolved styles after applying inheritance and overrides.
 */

namespace yap
{
    /**
     * @brief Represents a collection of optional style properties for UI elements.
     * 
     * The `StyleSheet` class allows defining styles such as visibility, size, position, alignment, padding,
     * background, border, and more. Each property is optional and can be set using the provided `With*` methods.
     */
    struct StyleSheet
    {
        Option<bool> Visibility;

        Option<PointerEvents> Events;

        Option<SizingRule> Size;
        Option<PositioningRule> Position;

        Option<ColorRGB> Foreground;

        Option<BoxBackground> Background;
        Option<BoxBackgroundTransparencyReference> BackgroundReference;
        Option<BoxBackgroundSizingRule> BackgroundSize;
        Option<BoxBackgroundPositioningRule> BackgroundPosition;

        Option<BoxBorder> Border;

        Option<BoxDirection> Direction;
        Option<BoxAlignment> Alignment;
        Option<BoxPadding> Padding;
        Option<float> Gap;

        StyleSheet &WithVisibility(bool visible)
        {
            Visibility = visible;
            return *this;
        }

        StyleSheet &WithEvents(PointerEvents pointerEvents)
        {
            Events = pointerEvents;
            return *this;
        }

        StyleSheet &WithSize(const AxisSizingRule &width, const AxisSizingRule &height)
        {
            Size = SizingRule(width, height);
            return *this;
        }

        StyleSheet &WithSize(const SizingRule &size)
        {
            Size = size;
            return *this;
        }

        StyleSheet &WithPosition(const PositioningRule &position)
        {
            Position = position;
            return *this;
        }

        StyleSheet &WithAlignment(const BoxAlignment &alignment)
        {
            Alignment = alignment;
            return *this;
        }

        StyleSheet &WithPadding(const BoxPadding &padding)
        {
            Padding = padding;
            return *this;
        }

        StyleSheet &WithGap(float gap)
        {
            Gap = gap;
            return *this;
        }

        StyleSheet &WithForeground(const ColorRGB &foreground)
        {
            Foreground = foreground;
            return *this;
        }

        StyleSheet &WithBackground(const BoxBackground &background)
        {
            Background = background;
            return *this;
        }

        StyleSheet &WithBackgroundReference(const BoxBackgroundTransparencyReference &reference)
        {
            BackgroundReference = reference;
            return *this;
        }

        StyleSheet &WithBackgroundSize(const BoxBackgroundSizingRule &size)
        {
            BackgroundSize = size;
            return *this;
        }

        StyleSheet &WithBackgroundPosition(const BoxBackgroundPositioningRule &positioning)
        {
            BackgroundPosition = positioning;
            return *this;
        }

        StyleSheet &WithDirection(BoxDirection direction)
        {
            Direction = direction;
            return *this;
        }

        StyleSheet &WithAlignment(BoxAxisAlignment horizontal, BoxAxisAlignment vertical)
        {
            Alignment = BoxAlignment(horizontal, vertical);
            return *this;
        }

        StyleSheet &WithBorder(const BoxBorder &border)
        {
            Border = border;
            return *this;
        }
    };

    /**
     * @brief Represents the resolved styles for a UI element after inheritance and overrides.
     * 
     * The `ComputedStyleSheet` class contains the final computed styles for a UI element, including
     * visibility, size, position, alignment, padding, background, border, and more.
     */
    struct ComputedStyleSheet
    {
        bool Visibility = true;

        PointerEvents Events = PointerEvents::Auto;

        SizingRule Size;
        PositioningRule Position;

        BoxPadding Padding;
        BoxAlignment Alignment;
        BoxDirection Direction = BoxDirection::Row;

        BoxBackground Background;
        BoxBackgroundTransparencyReference BackgroundReference;
        BoxBackgroundSizingRule BackgroundSize;
        BoxBackgroundPositioningRule BackgroundPosition;

        BoxBorder Border;

        ColorRGB Foreground;

        float Gap = 0.0f;

        ComputedStyleSheet()
        {
        }

        void Inherit(const ComputedStyleSheet& style)
        {
            Events = style.Events;
            Visibility = style.Visibility;
            Foreground = style.Foreground;
        }

        void Override(const StyleSheet& style)
        {
            if (style.Visibility)
            {
                Visibility = *style.Visibility;
            }

            if (style.Events)
            {
                Events = *style.Events;
            }

            if (style.Size)
            {
                Size = *style.Size;
            }

            if (style.Position)
            {
                Position = *style.Position;
            }

            if (style.Direction)
            {
                Direction = *style.Direction;
            }

            if (style.Alignment)
            {
                Alignment = *style.Alignment;
            }

            if (style.Padding)
            {
                Padding = *style.Padding;
            }

            if (style.Gap)
            {
                Gap = *style.Gap;
            }

            if (style.Foreground)
            {
                Foreground = *style.Foreground;
            }

            if (style.Background)
            {
                Background = *style.Background;
            }

            if (style.BackgroundReference)
            {
                BackgroundReference = *style.BackgroundReference;
            }

            if (style.BackgroundSize)
            {
                BackgroundSize = *style.BackgroundSize;
            }

            if (style.BackgroundPosition)
            {
                BackgroundPosition = *style.BackgroundPosition;
            }

            if (style.Border)
            {
                Border = *style.Border;
            }
        }

        void Reset()
        {
            Visibility = true;
            Events = PointerEvents::Auto;
            Size = SizingRule();
            Position = PositioningRule();
            Alignment = BoxAlignment();
            Direction = BoxDirection::Row;
            Padding = BoxPadding();
            Gap = 0.0f;
            Foreground = ColorRGB(0.0f, 0.0f, 0.0f);
            Background = BoxBackground();
            BackgroundReference = BoxBackgroundTransparencyReference();
            BackgroundPosition = BoxBackgroundPositioningRule();
            Border = BoxBorder();
        }
    };
}