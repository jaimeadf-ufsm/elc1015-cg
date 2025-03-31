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

    enum class BoxAlignmentRule
    {
        Start,
        Center,
        End
    };

    class BoxPadding
    {
    public:
        int Left;
        int Right;
        int Top;
        int Bottom;

        BoxPadding()
            : Left(0), Right(0), Top(0), Bottom(0) {}

        BoxPadding(int all)
            : Left(all), Right(all), Top(all), Bottom(all) {}

        BoxPadding(int horizontal, int vertical) : 
            Left(horizontal), Right(horizontal), Top(vertical), Bottom(vertical) {}

        BoxPadding(int left, int right, int top, int bottom) :
            Left(left), Right(right), Top(top), Bottom(bottom) {}
    
        int GetTotalPaddingAlongAxis(Axis axis) const
        {
            return (axis == Axis::X) ? Left + Right : Top + Bottom;
        }

        int GetStartPaddingAlongAxis(Axis axis) const
        {
            return (axis == Axis::X) ? Left : Top;
        }

        int GetEndPaddingAlongAxis(Axis axis) const
        {
            return (axis == Axis::X) ? Right : Bottom;
        }
    };

    class Box : public Element
    {
    private:
        std::vector<std::shared_ptr<Element>> m_Children;

    public:
        BoxDirection Direction = BoxDirection::Row;
        BoxPadding Padding = BoxPadding(0);

        BoxAlignmentRule HorizontalAlignment = BoxAlignmentRule::Start;
        BoxAlignmentRule VerticalAlignment = BoxAlignmentRule::Start;

        int ChildrenGap = 0;

        Box()
        {
        }

        void ComputeIndependentDimensions() override
        {
            Element::ComputeIndependentDimensions();

            Axis primaryAxis = GetDirectionPrimaryAxis();
            Axis secondaryAxis = GetDirectionSecondaryAxis();

            int contentPrimarySize = 0;
            int contentSecondarySize = 0;

            for (const auto& child : m_Children)
            {
                child->ComputeIndependentDimensions();

                if (child->GetSizeAlongAxis(primaryAxis).IsIndependent())
                {
                    contentPrimarySize += child->GetViewportSizeAlongAxis(primaryAxis);
                }

                if (child->Height.IsIndependent())
                {
                    contentSecondarySize = std::max(
                        contentSecondarySize,
                        child->GetViewportSizeAlongAxis(secondaryAxis)
                    );
                }
            }

            if (GetSizeAlongAxis(primaryAxis).IsFit())
            {
                int viewportPrimarySize = contentPrimarySize;
                viewportPrimarySize += Padding.GetTotalPaddingAlongAxis(primaryAxis);
                viewportPrimarySize += (m_Children.size() - 1) * ChildrenGap;

                SetViewportSizeAlongAxis(primaryAxis, viewportPrimarySize);
            }

            if (GetSizeAlongAxis(secondaryAxis).IsFit())
            {
                int viewportSecondarySize = contentSecondarySize;
                viewportSecondarySize += Padding.GetTotalPaddingAlongAxis(secondaryAxis);

                SetViewportSizeAlongAxis(secondaryAxis, viewportSecondarySize);
            }
        }

        void ComputeResponsiveDimensions() override
        {
            Element::ComputeResponsiveDimensions();

            Axis primaryAxis = GetDirectionPrimaryAxis();
            Axis secondaryAxis = GetDirectionSecondaryAxis();

            int remainingPrimarySize = GetViewportSizeAlongAxis(primaryAxis);
            int remainingSecondarySize = GetViewportSizeAlongAxis(secondaryAxis);

            remainingPrimarySize -= Padding.GetTotalPaddingAlongAxis(primaryAxis);
            remainingSecondarySize -= Padding.GetTotalPaddingAlongAxis(secondaryAxis);

            remainingPrimarySize -= (m_Children.size() - 1) * ChildrenGap;

            int fillableChildrenCount = 0;

            for (auto& child : m_Children)
            {
                remainingPrimarySize -= child->ViewportWidth;

                if (child->GetSizeAlongAxis(primaryAxis).IsFill())
                {
                    fillableChildrenCount++;
                }
            }

            for (auto& child : m_Children)
            {
                if (child->GetSizeAlongAxis(primaryAxis).IsFill())
                {
                    child->SetViewportSizeAlongAxis(
                        primaryAxis,
                        remainingPrimarySize / fillableChildrenCount
                    );
                }

                if (child->GetSizeAlongAxis(secondaryAxis).IsFill())
                {
                    child->SetViewportSizeAlongAxis(
                        secondaryAxis,
                        remainingSecondarySize
                    );
                }
            }
        }

        void ComputePosition() override
        {
            Element::ComputePosition();

            Axis primaryAxis = GetDirectionPrimaryAxis();
            Axis secondaryAxis = GetDirectionSecondaryAxis();

            int primaryOffset = GetViewportPositionAlongAxis(primaryAxis);
            int primaryContentSize = 0;

            for (const auto& child : m_Children)
            {
                primaryContentSize += child->GetViewportSizeAlongAxis(primaryAxis);
            }

            switch (GetAlignmentAlongAxis(primaryAxis))
            {
                case BoxAlignmentRule::Start:
                    primaryOffset += Padding.GetStartPaddingAlongAxis(primaryAxis);
                    break;
                case BoxAlignmentRule::Center:
                    primaryOffset += std::max(
                        Padding.GetStartPaddingAlongAxis(primaryAxis),
                        primaryContentSize / 2
                    );
                    break;
                case BoxAlignmentRule::End:
                    primaryOffset += GetViewportSizeAlongAxis(primaryAxis);
                    primaryOffset -= primaryContentSize;
                    primaryOffset -= Padding.GetEndPaddingAlongAxis(primaryAxis);
                    break;
            }

            for (auto& child : m_Children)
            {
                int secondaryOffset = GetViewportPositionAlongAxis(secondaryAxis);

                switch (GetAlignmentAlongAxis(secondaryAxis))
                {
                case BoxAlignmentRule::Start:
                    secondaryOffset += Padding.GetStartPaddingAlongAxis(secondaryAxis);
                    break;
                case BoxAlignmentRule::Center:
                    secondaryOffset += child->GetViewportSizeAlongAxis(secondaryAxis) / 2;
                    break;
                case BoxAlignmentRule::End:
                    secondaryOffset += GetViewportSizeAlongAxis(secondaryAxis);
                    secondaryOffset -= Padding.GetEndPaddingAlongAxis(secondaryAxis);
                    secondaryOffset -= child->GetViewportSizeAlongAxis(secondaryAxis);
                    break;
                }

                child->SetViewportPositionAlongAxis(primaryAxis, primaryOffset);
                child->SetViewportPositionAlongAxis(secondaryAxis, secondaryOffset);

                primaryOffset += child->GetViewportSizeAlongAxis(primaryAxis);
                primaryOffset += ChildrenGap;
            }
        }

        void Draw(RenderingContext& context) override
        {
            Element::Draw(context);

            for (const auto& child : m_Children)
            {
                child->Draw(context);
            }
        }

        void AddChild(std::shared_ptr<Element> child)
        {
            m_Children.push_back(child);
        }

        void RemoveChild(std::shared_ptr<Element> child)
        {
            m_Children.erase(
                std::remove(m_Children.begin(), m_Children.end(), child),
                m_Children.end()
            );
        }

        const std::vector<std::shared_ptr<Element>>& GetChildren() const
        {
            return m_Children;
        }

        BoxAlignmentRule GetAlignmentAlongAxis(Axis axis)
        {
            return (axis == Axis::X ? HorizontalAlignment : VerticalAlignment);
        }
    
    private:
        Axis GetDirectionPrimaryAxis()
        {
            return (Direction == BoxDirection::Row ? Axis::X : Axis::Y);
        }

        Axis GetDirectionSecondaryAxis()
        {
            return GetAxisComplement(GetDirectionPrimaryAxis());
        }
    };
}