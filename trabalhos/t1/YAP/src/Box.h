#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "Element.h"
#include "BoxStyle.h"

namespace yap
{
    class Box : public Element
    {
    public:
        BoxStyle Style;
        BoxComputedStyle ComputedStyle;

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

        void ComputeStyle(const BoxComputedStyle& parentStyle)
        {
            ComputedStyle.Reset();
            ComputedStyle.Inherit(parentStyle);
            ComputedStyle.Override(Style);

            for (auto& child : Children)
            {
                std::shared_ptr<Box> box = std::dynamic_pointer_cast<Box>(child);

                if (box)
                {
                    box->ComputeStyle(ComputedStyle);
                }
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

                std::shared_ptr<Box> box = std::dynamic_pointer_cast<Box>(child);

                if (!box)
                {
                    continue;
                }

                if (!box->ComputedStyle.Position.IsStatic())
                {
                    continue;
                }

                if (box->ComputedStyle.Size.GetSizeAlongAxis(primaryAxis).IsIndependent())
                {
                    contentPrimarySize += box->ViewportSize.GetValueAlongAxis(primaryAxis);
                }

                if (box->ComputedStyle.Size.GetSizeAlongAxis(secondaryAxis).IsIndependent())
                {
                    contentSecondarySize = std::max(
                        contentSecondarySize,
                        box->ViewportSize.GetValueAlongAxis(secondaryAxis)
                    );
                }

                contentPrimarySize += ComputedStyle.Gap;
            }

            if (Size.GetSizeAlongAxis(primaryAxis).IsFit())
            {
                float viewportPrimarySize = contentPrimarySize;
                viewportPrimarySize += ComputedStyle.Padding.GetTotalPaddingAlongAxis(primaryAxis);

                ViewportSize.SetValueAlongAxis(primaryAxis, viewportPrimarySize);
            }

            if (Size.GetSizeAlongAxis(secondaryAxis).IsFit())
            {
                float viewportSecondarySize = contentSecondarySize;
                viewportSecondarySize += ComputedStyle.Padding.GetTotalPaddingAlongAxis(secondaryAxis);

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

            remainingPrimarySize -= ComputedStyle.Padding.GetTotalPaddingAlongAxis(primaryAxis);
            remainingSecondarySize -= ComputedStyle.Padding.GetTotalPaddingAlongAxis(secondaryAxis);

            int staticFillableChildrenCount = 0.0f;

            for (const auto& child : Children)
            {
                if (!child->Position.IsStatic())
                {
                    continue;
                }

                remainingPrimarySize -= child->ViewportSize.GetValueAlongAxis(primaryAxis);
                remainingPrimarySize -= ComputedStyle.Gap;

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
                primaryContentSize += ComputedStyle.Gap;
            }

            switch (ComputedStyle.Alignment.GetAlignmentAlongAxis(primaryAxis))
            {
                case LayoutAxisAlignment::Start:
                    primaryOffset += ComputedStyle.Padding.GetStartPaddingAlongAxis(primaryAxis);
                    break;
                case LayoutAxisAlignment::Center:
                    primaryOffset += std::max(
                        ComputedStyle.Padding.GetStartPaddingAlongAxis(primaryAxis),
                        (
                            ViewportSize.GetValueAlongAxis(primaryAxis) -
                            primaryContentSize
                        ) / 2.0f
                    );
                    break;
                case LayoutAxisAlignment::End:
                    primaryOffset += ViewportSize.GetValueAlongAxis(primaryAxis);
                    primaryOffset -= primaryContentSize;
                    primaryOffset -= ComputedStyle.Padding.GetEndPaddingAlongAxis(primaryAxis);
                    break;
            }

            for (auto& child : Children)
            {
                float secondaryOffset = ViewportPosition.GetValueAlongAxis(secondaryAxis);

                switch (ComputedStyle.Alignment.GetAlignmentAlongAxis(secondaryAxis))
                {
                case LayoutAxisAlignment::Start:
                    secondaryOffset += ComputedStyle.Padding.GetStartPaddingAlongAxis(secondaryAxis);
                    break;
                case LayoutAxisAlignment::Center:
                    secondaryOffset += (
                        ViewportSize.GetValueAlongAxis(secondaryAxis) -
                        child->ViewportSize.GetValueAlongAxis(secondaryAxis)
                    ) / 2.0f;
                    break;
                case LayoutAxisAlignment::End:
                    secondaryOffset += ViewportSize.GetValueAlongAxis(secondaryAxis);
                    secondaryOffset -= ComputedStyle.Padding.GetEndPaddingAlongAxis(secondaryAxis);
                    secondaryOffset -= child->ViewportSize.GetValueAlongAxis(secondaryAxis);
                    break;
                }

                switch (child->Position.GetMode())
                {
                    case PositioningMode::Static:
                        child->ViewportPosition.SetValueAlongAxis(primaryAxis, primaryOffset);
                        child->ViewportPosition.SetValueAlongAxis(secondaryAxis, secondaryOffset);

                        primaryOffset += child->ViewportSize.GetValueAlongAxis(primaryAxis);
                        primaryOffset += ComputedStyle.Gap;
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
            // context.Color(Background);
            // context.FillRectangle(ViewportPosition, ViewportSize);

            // for (const auto& child : Children)
            // {
            //     child->Draw(context);
            // }
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
            return (ComputedStyle.Direction == BoxDirection::Row ? Axis::X : Axis::Y);
        }

        Axis GetDirectionSecondaryAxis()
        {
            return GetComplementAxis(GetDirectionPrimaryAxis());
        }
    };
}