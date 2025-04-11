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
        Option<bool> Visibility; ///< Optional visibility of the element.
        Option<PointerEvents> Events; ///< Optional pointer events behavior.
        Option<SizingRule> Size; ///< Optional sizing rules for the element.
        Option<PositioningRule> Position; ///< Optional positioning rules for the element.
        Option<ColorRGB> Foreground; ///< Optional foreground color.
        Option<BoxBackground> Background; ///< Optional background properties.
        Option<BoxBackgroundTransparencyReference> BackgroundReference; ///< Optional background transparency reference.
        Option<BoxBackgroundSizingRule> BackgroundSize; ///< Optional background sizing rules.
        Option<BoxBackgroundPositioningRule> BackgroundPosition; ///< Optional background positioning rules.
        Option<BoxBorder> Border; ///< Optional border properties.
        Option<BoxDirection> Direction; ///< Optional layout direction.
        Option<BoxAlignment> Alignment; ///< Optional alignment properties.
        Option<BoxPadding> Padding; ///< Optional padding properties.
        Option<float> Gap; ///< Optional gap between elements.

        /**
         * @brief Sets the visibility of the element.
         * @param visible The visibility state.
         * @return Reference to the updated `StyleSheet`.
         */
        StyleSheet &WithVisibility(bool visible)
        {
            Visibility = visible;
            return *this;
        }

        /**
         * @brief Sets the pointer events behavior.
         * @param pointerEvents The pointer events behavior.
         * @return Reference to the updated `StyleSheet`.
         */
        StyleSheet &WithEvents(PointerEvents pointerEvents)
        {
            Events = pointerEvents;
            return *this;
        }

        /**
         * @brief Sets the size of the element using width and height rules.
         * @param width The width sizing rule.
         * @param height The height sizing rule.
         * @return Reference to the updated `StyleSheet`.
         */
        StyleSheet &WithSize(const AxisSizingRule &width, const AxisSizingRule &height)
        {
            Size = SizingRule(width, height);
            return *this;
        }

        /**
         * @brief Sets the size of the element using a sizing rule.
         * @param size The sizing rule.
         * @return Reference to the updated `StyleSheet`.
         */
        StyleSheet &WithSize(const SizingRule &size)
        {
            Size = size;
            return *this;
        }

        /**
         * @brief Sets the position of the element.
         * @param position The positioning rule.
         * @return Reference to the updated `StyleSheet`.
         */
        StyleSheet &WithPosition(const PositioningRule &position)
        {
            Position = position;
            return *this;
        }

        /**
         * @brief Sets the alignment of the element.
         * @param alignment The alignment properties.
         * @return Reference to the updated `StyleSheet`.
         */
        StyleSheet &WithAlignment(const BoxAlignment &alignment)
        {
            Alignment = alignment;
            return *this;
        }

        /**
         * @brief Sets the padding of the element.
         * @param padding The padding properties.
         * @return Reference to the updated `StyleSheet`.
         */
        StyleSheet &WithPadding(const BoxPadding &padding)
        {
            Padding = padding;
            return *this;
        }

        /**
         * @brief Sets the gap between elements.
         * @param gap The gap value.
         * @return Reference to the updated `StyleSheet`.
         */
        StyleSheet &WithGap(float gap)
        {
            Gap = gap;
            return *this;
        }

        /**
         * @brief Sets the foreground color of the element.
         * @param foreground The foreground color.
         * @return Reference to the updated `StyleSheet`.
         */
        StyleSheet &WithForeground(const ColorRGB &foreground)
        {
            Foreground = foreground;
            return *this;
        }

        /**
         * @brief Sets the background properties of the element.
         * @param background The background properties.
         * @return Reference to the updated `StyleSheet`.
         */
        StyleSheet &WithBackground(const BoxBackground &background)
        {
            Background = background;
            return *this;
        }

        /**
         * @brief Sets the background transparency reference.
         * @param reference The transparency reference.
         * @return Reference to the updated `StyleSheet`.
         */
        StyleSheet &WithBackgroundReference(const BoxBackgroundTransparencyReference &reference)
        {
            BackgroundReference = reference;
            return *this;
        }

        /**
         * @brief Sets the background sizing rules.
         * @param size The background sizing rules.
         * @return Reference to the updated `StyleSheet`.
         */
        StyleSheet &WithBackgroundSize(const BoxBackgroundSizingRule &size)
        {
            BackgroundSize = size;
            return *this;
        }

        /**
         * @brief Sets the background positioning rules.
         * @param positioning The background positioning rules.
         * @return Reference to the updated `StyleSheet`.
         */
        StyleSheet &WithBackgroundPosition(const BoxBackgroundPositioningRule &positioning)
        {
            BackgroundPosition = positioning;
            return *this;
        }

        /**
         * @brief Sets the layout direction of the element.
         * @param direction The layout direction.
         * @return Reference to the updated `StyleSheet`.
         */
        StyleSheet &WithDirection(BoxDirection direction)
        {
            Direction = direction;
            return *this;
        }

        /**
         * @brief Sets the alignment of the element using horizontal and vertical alignment.
         * @param horizontal The horizontal alignment.
         * @param vertical The vertical alignment.
         * @return Reference to the updated `StyleSheet`.
         */
        StyleSheet &WithAlignment(BoxAxisAlignment horizontal, BoxAxisAlignment vertical)
        {
            Alignment = BoxAlignment(horizontal, vertical);
            return *this;
        }

        /**
         * @brief Sets the border properties of the element.
         * @param border The border properties.
         * @return Reference to the updated `StyleSheet`.
         */
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
        bool Visibility = true; ///< Visibility of the element.
        PointerEvents Events = PointerEvents::Auto; ///< Pointer events behavior.
        SizingRule Size; ///< Sizing rules for the element.
        PositioningRule Position; ///< Positioning rules for the element.
        BoxPadding Padding; ///< Padding properties.
        BoxAlignment Alignment; ///< Alignment properties.
        BoxDirection Direction = BoxDirection::Row; ///< Layout direction.
        BoxBackground Background; ///< Background properties.
        BoxBackgroundTransparencyReference BackgroundReference; ///< Background transparency reference.
        BoxBackgroundSizingRule BackgroundSize; ///< Background sizing rules.
        BoxBackgroundPositioningRule BackgroundPosition; ///< Background positioning rules.
        BoxBorder Border; ///< Border properties.
        ColorRGB Foreground; ///< Foreground color.
        float Gap = 0.0f; ///< Gap between elements.

        /**
         * @brief Default constructor for `ComputedStyleSheet`.
         */
        ComputedStyleSheet()
        {
        }

        /**
         * @brief Inherits styles from another `ComputedStyleSheet`.
         * @param style The `ComputedStyleSheet` to inherit from.
         */
        void Inherit(const ComputedStyleSheet& style)
        {
            Events = style.Events;
            Visibility = style.Visibility;
            Foreground = style.Foreground;
        }

        /**
         * @brief Overrides styles using a `StyleSheet`.
         * @param style The `StyleSheet` containing the styles to override.
         */
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

        /**
         * @brief Resets all styles to their default values.
         */
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