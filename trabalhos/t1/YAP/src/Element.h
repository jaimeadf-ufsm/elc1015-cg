#pragma once

#include "SizingRule.h"
#include "PositioningRule.h"
#include "ColorRGBA.h"

#include "RenderingContext.h"

namespace yap
{
    class Element
    {
    public:
        int ZIndex;

        SizingRule Size;
        PositioningRule Position;

        ColorRGBA Background;

        int ViewportWidth;
        int ViewportHeight;

        int ViewportX;
        int ViewportY;

        virtual void ComputeFixedDimensions()
        {
            switch (Size.Width.Mode)
            {
                case AxisSizingMode::Fixed:
                    ViewportWidth = Size.Width.Value;
                    break;
                case AxisSizingMode::Fit:
                    ViewportWidth = 0; // To be computed later by implementing element
                    break;
                case AxisSizingMode::Fill:
                    ViewportWidth = 0; // To be computed later by parent element
                    break;
            }

            switch (Size.Height.Mode)
            {
                case AxisSizingMode::Fixed:
                    ViewportHeight = Size.Height.Value;
                    break;
                case AxisSizingMode::Fit:
                    ViewportHeight = 0; // To be computed later by implementing element
                    break;
                case AxisSizingMode::Fill:
                    ViewportHeight = 0; // To be computed later by parent element
                    break;
            }
        }

        virtual void ComputeResponsiveDimensions()
        {
            return;
        }

        virtual void ComputePosition()
        {
            switch (Position.Kind)
            {
                case PositioningMode::Static:
                    ViewportX = 0;
                    ViewportY = 0;
                    break;
                case PositioningMode::Float:
                    ViewportX = Position.X;
                    ViewportY = Position.Y;
                    break;
            }
        }

        virtual void Draw(RenderingContext& context)
        {
            context.Color(Background);
            context.FillRectangle(ViewportX, ViewportY, ViewportWidth, ViewportHeight);
        }
    };
}