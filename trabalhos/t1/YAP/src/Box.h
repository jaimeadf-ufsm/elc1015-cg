#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "Element.h"

namespace yap
{
    enum class BoxDirection
    {
        Row,
        Column
    };

    enum class BoxAxisAlignment
    {
        Start,
        Center,
        End
    };

    struct BoxAlignment
    {
        BoxAxisAlignment Horizontal;
        BoxAxisAlignment Vertical;

        BoxAlignment() :
            Horizontal(BoxAxisAlignment::Start), Vertical(BoxAxisAlignment::Start) {}

        BoxAlignment(BoxAxisAlignment horizontal, BoxAxisAlignment vertical) :
            Horizontal(horizontal), Vertical(vertical) {}

        BoxAxisAlignment GetAlignmentAlongAxis(Axis axis)
        {
            return (axis == Axis::X ? Horizontal : Vertical);
        }
    };

    class BoxPadding
    {
    public:
        float Left;
        float Right;
        float Top;
        float Bottom;

        BoxPadding()
            : Left(0), Right(0), Top(0), Bottom(0) {}

        BoxPadding(float all)
            : Left(all), Right(all), Top(all), Bottom(all) {}

        BoxPadding(float horizontal, float vertical) : 
            Left(horizontal), Right(horizontal), Top(vertical), Bottom(vertical) {}

        BoxPadding(float left, float right, float top, float bottom) :
            Left(left), Right(right), Top(top), Bottom(bottom) {}
    
        float GetTotalPaddingAlongAxis(Axis axis) const
        {
            return (axis == Axis::X) ? Left + Right : Top + Bottom;
        }

        float GetStartPaddingAlongAxis(Axis axis) const
        {
            return (axis == Axis::X) ? Left : Top;
        }

        float GetEndPaddingAlongAxis(Axis axis) const
        {
            return (axis == Axis::X) ? Right : Bottom;
        }
    };

    class Box : public Element
    {
    private:

    public:
        BoxDirection Direction = BoxDirection::Row;
        BoxAlignment Alignment = BoxAlignment();

        BoxPadding Padding = BoxPadding();
        float Gap = 0;

        ColorRGB Background = ColorRGB();

        std::vector<std::shared_ptr<Element>> Children;

        Box()
        {
        }

        void ProcessMouseMove(Mouse& mouse) override
        {
            Element::ProcessMouseMove(mouse);

            for (const auto& child : Children)
            {
                child->ProcessMouseMove(mouse);
            }
        }

        void ProcessMouseUp(Mouse& mouse, MouseButton button) override
        {
            Element::ProcessMouseUp(mouse, button);

            for (const auto& child : Children)
            {
                child->ProcessMouseUp(mouse, button);
            }
        }

        void ProcessMouseDown(Mouse& mouse, MouseButton button) override
        {
            Element::ProcessMouseDown(mouse, button);

            for (const auto& child : Children)
            {
                child->ProcessMouseDown(mouse, button);
            }
        }

        void ProcessMouseScroll(Mouse& mouse, MouseScrollDirection direction) override
        {
            Element::ProcessMouseScroll(mouse, direction);

            for (const auto& child : Children)
            {
                child->ProcessMouseScroll(mouse, direction);
            }
        }

        void ProcessKeyboardDown(Keyboard& keyboard, KeyboardKey key) override
        {
            Element::ProcessKeyboardDown(keyboard, key);

            for (const auto& child : Children)
            {
                child->ProcessKeyboardDown(keyboard, key);
            }
        }

        void ProcessKeyboardUp(Keyboard& keyboard, KeyboardKey key) override
        {
            Element::ProcessKeyboardUp(keyboard, key);

            for (auto& child : Children)
            {
                child->ProcessKeyboardUp(keyboard, key);
            }
        }

        void Animate() override
        {
            Element::Animate();

            for (const auto& child : Children)
            {
                child->Animate();
            }
        }

        void ComputeIndependentDimensions() override
        {
            Element::ComputeIndependentDimensions();

            Axis primaryAxis = GetDirectionPrimaryAxis();
            Axis secondaryAxis = GetDirectionSecondaryAxis();

            float contentPrimarySize = 0;
            float contentSecondarySize = 0;


            for (const auto& child : Children)
            {
                child->ComputeIndependentDimensions();

                if (!child->Position.IsStatic())
                {
                    continue;
                }

                if (child->Size.GetSizeAlongAxis(primaryAxis).IsIndependent())
                {
                    contentPrimarySize += child->ViewportSize.GetValueAlongAxis(primaryAxis);
                }

                if (child->Size.GetSizeAlongAxis(secondaryAxis).IsIndependent())
                {
                    contentSecondarySize = std::max(
                        contentSecondarySize,
                        child->ViewportSize.GetValueAlongAxis(secondaryAxis)
                    );
                }

                contentPrimarySize += Gap;
            }

            if (Size.GetSizeAlongAxis(primaryAxis).IsFit())
            {
                float viewportPrimarySize = contentPrimarySize;
                viewportPrimarySize += Padding.GetTotalPaddingAlongAxis(primaryAxis);

                ViewportSize.SetValueAlongAxis(primaryAxis, viewportPrimarySize);
            }

            if (Size.GetSizeAlongAxis(secondaryAxis).IsFit())
            {
                float viewportSecondarySize = contentSecondarySize;
                viewportSecondarySize += Padding.GetTotalPaddingAlongAxis(secondaryAxis);

                ViewportSize.SetValueAlongAxis(secondaryAxis, viewportSecondarySize);
            }
        }

        void ComputeResponsiveDimensions() override
        {
            Element::ComputeResponsiveDimensions();

            Axis primaryAxis = GetDirectionPrimaryAxis();
            Axis secondaryAxis = GetDirectionSecondaryAxis();

            float remainingPrimarySize = ViewportSize.GetValueAlongAxis(primaryAxis);
            float remainingSecondarySize = ViewportSize.GetValueAlongAxis(secondaryAxis);

            remainingPrimarySize -= Padding.GetTotalPaddingAlongAxis(primaryAxis);
            remainingSecondarySize -= Padding.GetTotalPaddingAlongAxis(secondaryAxis);

            int staticFillableChildrenCount = 0.0f;

            for (const auto& child : Children)
            {
                if (!child->Position.IsStatic())
                {
                    continue;
                }

                remainingPrimarySize -= child->ViewportSize.GetValueAlongAxis(primaryAxis);
                remainingPrimarySize -= Gap;

                if (child->Size.GetSizeAlongAxis(primaryAxis).IsFill())
                {
                    staticFillableChildrenCount++;
                }
            }

            for (auto& child : Children)
            {
                if (child->Position.IsStatic())
                {
                    if (child->Size.GetSizeAlongAxis(primaryAxis).IsFill())
                    {
                        child->ViewportSize.SetValueAlongAxis(
                            primaryAxis,
                            remainingPrimarySize / staticFillableChildrenCount
                        );
                    }

                    if (child->Size.GetSizeAlongAxis(secondaryAxis).IsFill())
                    {
                        child->ViewportSize.SetValueAlongAxis(
                            secondaryAxis,
                            remainingSecondarySize
                        );
                    }
                }
                else
                {
                    if (child->Size.Width.IsFill())
                    {
                        child->ViewportSize.X = ViewportSize.X;
                    }

                    if (child->Size.Height.IsFill())
                    {
                        child->ViewportSize.Y = ViewportSize.Y;
                    }
                }
            }
        }

        void ComputePosition() override
        {
            Element::ComputePosition();

            Axis primaryAxis = GetDirectionPrimaryAxis();
            Axis secondaryAxis = GetDirectionSecondaryAxis();

            float primaryOffset = ViewportPosition.GetValueAlongAxis(primaryAxis);
            float primaryContentSize = 0.0f;

            for (const auto& child : Children)
            {
                if (!child->Position.IsStatic())
                {
                    continue;
                }

                primaryContentSize += child->ViewportSize.GetValueAlongAxis(primaryAxis);
                primaryContentSize += Gap;
            }

            switch (Alignment.GetAlignmentAlongAxis(primaryAxis))
            {
                case BoxAxisAlignment::Start:
                    primaryOffset += Padding.GetStartPaddingAlongAxis(primaryAxis);
                    break;
                case BoxAxisAlignment::Center:
                    primaryOffset += std::max(
                        Padding.GetStartPaddingAlongAxis(primaryAxis),
                        (
                            ViewportSize.GetValueAlongAxis(primaryAxis) -
                            primaryContentSize
                        ) / 2.0f
                    );
                    break;
                case BoxAxisAlignment::End:
                    primaryOffset += ViewportSize.GetValueAlongAxis(primaryAxis);
                    primaryOffset -= primaryContentSize;
                    primaryOffset -= Padding.GetEndPaddingAlongAxis(primaryAxis);
                    break;
            }

            for (auto& child : Children)
            {
                float secondaryOffset = ViewportPosition.GetValueAlongAxis(secondaryAxis);

                switch (Alignment.GetAlignmentAlongAxis(secondaryAxis))
                {
                case BoxAxisAlignment::Start:
                    secondaryOffset += Padding.GetStartPaddingAlongAxis(secondaryAxis);
                    break;
                case BoxAxisAlignment::Center:
                    secondaryOffset += (
                        ViewportSize.GetValueAlongAxis(secondaryAxis) -
                        child->ViewportSize.GetValueAlongAxis(secondaryAxis)
                    ) / 2.0f;
                    break;
                case BoxAxisAlignment::End:
                    secondaryOffset += ViewportSize.GetValueAlongAxis(secondaryAxis);
                    secondaryOffset -= Padding.GetEndPaddingAlongAxis(secondaryAxis);
                    secondaryOffset -= child->ViewportSize.GetValueAlongAxis(secondaryAxis);
                    break;
                }

                switch (child->Position.GetMode())
                {
                    case PositioningMode::Static:
                        child->ViewportPosition.SetValueAlongAxis(primaryAxis, primaryOffset);
                        child->ViewportPosition.SetValueAlongAxis(secondaryAxis, secondaryOffset);

                        primaryOffset += child->ViewportSize.GetValueAlongAxis(primaryAxis);
                        primaryOffset += Gap;
                        break;
                    case PositioningMode::Relative:
                        child->ViewportPosition = ViewportPosition + child->Position.GetOffset();
                        break;
                    default:
                        break;
                }

                child->ComputePosition();
            }
        }

        void Draw(RenderingContext& context) override
        {
            context.Color(Background);
            context.FillRectangle(ViewportPosition, ViewportSize);

            for (const auto& child : Children)
            {
                child->Draw(context);
            }
        }

        void AddChild(std::shared_ptr<Element> child)
        {
            Children.push_back(child);
        }

        void RemoveChild(std::shared_ptr<Element> child)
        {
            Children.erase(
                std::remove(Children.begin(), Children.end(), child),
                Children.end()
            );
        }

        const std::vector<std::shared_ptr<Element>>& GetChildren() const
        {
            return Children;
        }
    
    private:
        Axis GetDirectionPrimaryAxis()
        {
            return (Direction == BoxDirection::Row ? Axis::X : Axis::Y);
        }

        Axis GetDirectionSecondaryAxis()
        {
            return GetComplementAxis(GetDirectionPrimaryAxis());
        }
    };
}