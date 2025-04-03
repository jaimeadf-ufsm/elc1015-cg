#pragma once

#include "Option.h"
#include "SizingRule.h"
#include "PositioningRule.h"
#include "Padding.h"
#include "BoxBackground.h"
#include "BoxDirection.h"
#include "BoxAlignment.h"

namespace yap
{
    struct BoxStyle
    {
        Option<int> ZIndex;
        Option<SizingRule> Size;
        Option<PositioningRule> Position;
        Option<BoxPadding> Padding;
        Option<BoxBackground> Background;
        Option<BoxDirection> Direction;
        Option<BoxAlignment> Alignment;
        Option<float> Gap;
    };

    struct BoxComputedStyle
    {
        int ZIndex = 0;
        SizingRule Size;
        PositioningRule Position;
        BoxBackground Background;
        BoxPadding Padding;
        BoxAlignment Alignment;
        BoxDirection Direction = BoxDirection::Row;

        float Gap = 0.0f;

        BoxComputedStyle()
        {
        }

        void Inherit(const BoxComputedStyle& style)
        {
            ZIndex = style.ZIndex;
        }

        void Override(const BoxStyle& style)
        {
            if (style.ZIndex)
            {
                ZIndex = *style.ZIndex;
            }

            if (style.Size)
            {
                Size = *style.Size;
            }

            if (style.Position)
            {
                Position = *style.Position;
            }

            if (style.Padding)
            {
                Padding = *style.Padding;
            }

            if (style.Background)
            {
                Background = *style.Background;
            }

            if (style.Direction)
            {
                Direction = *style.Direction;
            }

            if (style.Alignment)
            {
                Alignment = *style.Alignment;
            }

            if (style.Gap)
            {
                Gap = *style.Gap;
            }
        }

        void Reset()
        {
            ZIndex = 0;
            Size = SizingRule();
            Position = PositioningRule();
            Background = BoxBackground();
            Padding = BoxPadding();
            Alignment = BoxAlignment();
            Direction = BoxDirection::Row;
            Gap = 0.0f;
        }
    };
}