#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "Element.h"

namespace yap
{
    enum class BoxDirection
    {
        Horizontal,
        Vertical
    };

    enum class BoxAxisAlignmentRule
    {
        Start,
        Center,
        End
    };

    class BoxAlignmentRule
    {
    public:
        BoxAxisAlignmentRule X;
        BoxAxisAlignmentRule Y;

        BoxAlignmentRule() : X(BoxAxisAlignmentRule::Start), Y(BoxAxisAlignmentRule::Start) {}
        BoxAlignmentRule(BoxAxisAlignmentRule x, BoxAxisAlignmentRule y) : X(x), Y(y) {}
    };

    class BoxPadding
    {
    public:
        int Left;
        int Right;
        int Top;
        int Bottom;

        BoxPadding() : Left(0), Right(0), Top(0), Bottom(0) {}
        BoxPadding(int all) : Left(all), Right(all), Top(all), Bottom(all) {}
        BoxPadding(int horizontal, int vertical) : Left(horizontal), Right(horizontal), Top(vertical), Bottom(vertical) {}
        BoxPadding(int left, int right, int top, int bottom) : Left(left), Right(right), Top(top), Bottom(bottom) {}
    };

    class Box : public Element
    {
    private:
        std::vector<std::shared_ptr<Element>> m_Children;

    public:
        BoxDirection Direction;
        BoxPadding Padding;

        BoxAlignmentRule ChildrenAlignment;
        int ChildrenGap;

        Box()
        {
        }

        void ComputeFixedDimensions() override
        {
            if (Direction != BoxDirection::Horizontal)
            {
                return;
            }

            Element::ComputeFixedDimensions();

            int totalWidth = 0;
            int totalHeight = 0;

            int maxHeight = 0;

            for (const auto& child : m_Children)
            {
                child->ComputeFixedDimensions();

                switch (child->Size.Width.Mode)
                {
                case AxisSizingMode::Fixed:
                case AxisSizingMode::Fit:
                    totalWidth += child->ViewportWidth;
                    break;
                case AxisSizingMode::Fill:
                    break;
                }

                switch (child->Size.Height.Mode)
                {
                case AxisSizingMode::Fixed:
                case AxisSizingMode::Fit:
                    maxHeight = std::max(maxHeight, child->ViewportHeight);
                    break;
                case AxisSizingMode::Fill:
                    break;
                }
            }

            if (Size.Width.Mode == AxisSizingMode::Fit)
            {
                ViewportWidth = totalWidth + Padding.Left + Padding.Right + (m_Children.size() - 1) * ChildrenGap;
            }

            if (Size.Height.Mode == AxisSizingMode::Fit)
            {
                ViewportHeight = maxHeight + Padding.Top + Padding.Bottom;
            }
        }

        void ComputeResponsiveDimensions() override
        {
            if (Direction != BoxDirection::Horizontal)
            {
                return;
            }

            int remainingWidth = ViewportWidth;
            int remainingHeight = ViewportHeight;

            remainingWidth -= Padding.Left + Padding.Right;
            remainingHeight -= Padding.Top + Padding.Bottom;

            remainingWidth -= (m_Children.size() - 1) * ChildrenGap;

            int fillableChildrenCount = 0;

            for (auto& child : m_Children)
            {
                remainingWidth -= child->ViewportWidth;

                if (child->Size.Width.Mode == AxisSizingMode::Fill)
                {
                    fillableChildrenCount++;
                }
            }

            for (auto& child : m_Children)
            {
                if (child->Size.Height.Mode == AxisSizingMode::Fill)
                {
                    child->ViewportHeight = remainingHeight;
                }

                if (child->Size.Width.Mode == AxisSizingMode::Fill)
                {
                    child->ViewportWidth = remainingWidth / fillableChildrenCount;
                }
            }
        }

        void ComputePosition() override
        {
            int offsetX = ViewportX + Padding.Left;
            int offsetY = ViewportY + Padding.Top;

            for (auto& child : m_Children)
            {
                child->ComputePosition();

                switch (ChildrenAlignment.X)
                {
                case BoxAxisAlignmentRule::Start:
                    child->ViewportX = offsetX;
                    break;
                case BoxAxisAlignmentRule::Center:
                    child->ViewportX = offsetX + (ViewportWidth - child->ViewportWidth) / 2;
                    break;
                case BoxAxisAlignmentRule::End:
                    child->ViewportX = offsetX + ViewportWidth - child->ViewportWidth;
                    break;
                }

                switch (ChildrenAlignment.Y)
                {
                case BoxAxisAlignmentRule::Start:
                    child->ViewportY = offsetY;
                    break;
                case BoxAxisAlignmentRule::Center:
                    child->ViewportY = offsetY + (ViewportHeight - child->ViewportHeight) / 2;
                    break;
                case BoxAxisAlignmentRule::End:
                    child->ViewportY = offsetY + ViewportHeight - child->ViewportHeight;
                    break;
                }

                offsetX += child->ViewportWidth + ChildrenGap;
            }

            offsetY += Padding.Top + Padding.Bottom;
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
            m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end());
        }

        const std::vector<std::shared_ptr<Element>>& GetChildren() const
        {
            return m_Children;
        }
    };
}