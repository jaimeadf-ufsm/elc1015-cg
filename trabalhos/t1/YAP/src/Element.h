#pragma once

#include "Axis.h"
#include "SizingRule.h"
#include "PositioningRule.h"
#include "ColorRGBA.h"

#include "RenderingContext.h"

namespace yap
{
    class Element
    {
    public:
        int ZIndex = 0;

        SizingRule Width = SizingRule::Fit();
        SizingRule Height = SizingRule::Fit();

        PositioningRule Position = PositioningRule::Static();

        ColorRGBA Background = ColorRGBA(0.0f, 0.0f, 0.0f, 0.0f);

        int ViewportWidth = 0;
        int ViewportHeight = 0;

        int ViewportX = 0;
        int ViewportY = 0;

        virtual void ComputeIndependentDimensions()
        {
            switch (Width.GetMode())
            {
                case SizingMode::Fixed:
                    ViewportWidth = Width.GetValue();
                    break;
                case SizingMode::Fit:
                    ViewportWidth = 0; // To be computed later by implementing element
                    break;
                case SizingMode::Fill:
                    ViewportWidth = 0; // To be computed later by parent element
                    break;
            }

            switch (Height.GetMode())
            {
                case SizingMode::Fixed:
                    ViewportHeight = Height.GetValue();
                    break;
                case SizingMode::Fit:
                    ViewportHeight = 0; // To be computed later by implementing element
                    break;
                case SizingMode::Fill:
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
            switch (Position.GetMode())
            {
                case PositioningMode::Static:
                    ViewportX = 0;
                    ViewportY = 0;
                    break;
                case PositioningMode::Float:
                    ViewportX = Position.GetX();
                    ViewportY = Position.GetY();
                    break;
            }
        }

        virtual void Draw(RenderingContext& context)
        {
            context.Color(Background);
            context.FillRectangle(ViewportX, ViewportY, ViewportWidth, ViewportHeight);
        }

        void SetSizeAlongAxis(Axis axis, SizingRule size)
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

        SizingRule GetSizeAlongAxis(Axis axis) const
        {
            return (axis == Axis::X) ? Width : Height;
        }

        void SetViewportSizeAlongAxis(Axis axis, int size)
        {
            if (axis == Axis::X)
            {
                ViewportWidth = size;
            }
            else
            {
                ViewportHeight = size;
            }
        }

        int GetViewportSizeAlongAxis(Axis axis) const
        {
            return (axis == Axis::X) ? ViewportWidth : ViewportHeight;
        }

        void SetViewportPositionAlongAxis(Axis axis, int position)
        {
            if (axis == Axis::X)
            {
                ViewportX = position;
            }
            else
            {
                ViewportY = position;
            }
        }

        int GetViewportPositionAlongAxis(Axis axis) const
        {
            return (axis == Axis::X) ? ViewportX : ViewportY;
        }
    };
}