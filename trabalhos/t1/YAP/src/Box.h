#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "Element.h"
#include "StyleSheet.h"

namespace yap
{
    class Box : public Element
    {
    private:
        std::shared_ptr<Bitmap> m_BufferBitmap;

    public:
        std::vector<std::shared_ptr<Element>> Children;

        Box() : m_BufferBitmap(std::make_shared<Bitmap>(0, 0))
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

        void Mount(const std::shared_ptr<Screen>& screen) override
        {
            Element::Mount(screen);

            for (const auto& child : Children)
            {
                child->Mount(screen);
            }
        }

        void Unmount() override
        {
            Element::Unmount();

            for (const auto& child : Children)
            {
                child->Unmount();
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

        void ComputeStyle(const ComputedStyleSheet& parentStyle)
        {
            Element::ComputeStyle(parentStyle);

            for (auto& child : Children)
            {
                child->ComputeStyle(ComputedStyle);
            }
        }

        void ComputeIndependentDimensions() override
        {
            Element::ComputeIndependentDimensions();

            Axis primaryAxis = GetDirectionPrimaryAxis();
            Axis secondaryAxis = GetDirectionSecondaryAxis();

            int staticChildrenCount = 0;

            float contentPrimarySize = 0;
            float contentSecondarySize = 0;

            for (const auto& child : Children)
            {
                child->ComputeIndependentDimensions();

                if (!child->ComputedStyle.Position.IsStatic())
                {
                    continue;
                }

                if (child->ComputedStyle.Size.GetSizeAlongAxis(primaryAxis).IsIndependent())
                {
                    contentPrimarySize += child->Size.GetValueAlongAxis(primaryAxis);
                }

                if (child->ComputedStyle.Size.GetSizeAlongAxis(secondaryAxis).IsIndependent())
                {
                    contentSecondarySize = std::max(
                        contentSecondarySize,
                        child->Size.GetValueAlongAxis(secondaryAxis)
                    );
                }

                staticChildrenCount++;
            }

            contentPrimarySize += ComputedStyle.Gap * (staticChildrenCount - 1);

            if (ComputedStyle.Size.GetSizeAlongAxis(primaryAxis).IsFit())
            {
                float viewportPrimarySize = contentPrimarySize;
                viewportPrimarySize += ComputedStyle.Padding.GetTotalPaddingAlongAxis(primaryAxis);

                Size.SetValueAlongAxis(primaryAxis, viewportPrimarySize);
            }

            if (ComputedStyle.Size.GetSizeAlongAxis(secondaryAxis).IsFit())
            {
                float viewportSecondarySize = contentSecondarySize;
                viewportSecondarySize += ComputedStyle.Padding.GetTotalPaddingAlongAxis(secondaryAxis);

                Size.SetValueAlongAxis(secondaryAxis, viewportSecondarySize);
            }
        }

        void ComputeResponsiveDimensions() override
        {
            Element::ComputeResponsiveDimensions();

            Axis primaryAxis = GetDirectionPrimaryAxis();
            Axis secondaryAxis = GetDirectionSecondaryAxis();

            float remainingPrimarySize = Size.GetValueAlongAxis(primaryAxis);
            float remainingSecondarySize = Size.GetValueAlongAxis(secondaryAxis);

            remainingPrimarySize -= ComputedStyle.Padding.GetTotalPaddingAlongAxis(primaryAxis);
            remainingSecondarySize -= ComputedStyle.Padding.GetTotalPaddingAlongAxis(secondaryAxis);

            int staticChildrenCount = 0;
            int staticFillableChildrenCount = 0;

            for (const auto& child : Children)
            {
                if (!child->ComputedStyle.Position.IsStatic())
                {
                    continue;
                }

                staticChildrenCount++;
                remainingPrimarySize -= child->Size.GetValueAlongAxis(primaryAxis);

                if (child->ComputedStyle.Size.GetSizeAlongAxis(primaryAxis).IsFill())
                {
                    staticFillableChildrenCount++;
                }
            }

            remainingPrimarySize -= ComputedStyle.Gap * (staticChildrenCount - 1);

            for (auto& child : Children)
            {
                if (child->ComputedStyle.Position.IsStatic())
                {
                    if (child->ComputedStyle.Size.GetSizeAlongAxis(primaryAxis).IsFill())
                    {
                        child->Size.SetValueAlongAxis(
                            primaryAxis,
                            remainingPrimarySize / staticFillableChildrenCount
                        );
                    }

                    if (child->ComputedStyle.Size.GetSizeAlongAxis(secondaryAxis).IsFill())
                    {
                        child->Size.SetValueAlongAxis(
                            secondaryAxis,
                            remainingSecondarySize
                        );
                    }
                }
                else
                {
                    if (child->ComputedStyle.Size.Width.IsFill())
                    {
                        child->Size.X = Size.X;
                    }

                    if (child->ComputedStyle.Size.Height.IsFill())
                    {
                        child->Size.Y = Size.Y;
                    }
                }

                child->ComputeResponsiveDimensions();
            }
        }

        void ComputePosition() override
        {
            Element::ComputePosition();

            Axis primaryAxis = GetDirectionPrimaryAxis();
            Axis secondaryAxis = GetDirectionSecondaryAxis();

            int staticChildrenCount = 0;

            float primaryOffset = Position.GetValueAlongAxis(primaryAxis);
            float primaryContentSize = 0.0f;

            for (const auto& child : Children)
            {
                if (!child->ComputedStyle.Position.IsStatic())
                {
                    continue;
                }
                
                staticChildrenCount++;
                primaryContentSize += child->Size.GetValueAlongAxis(primaryAxis);
            }

            primaryContentSize += ComputedStyle.Gap * (staticChildrenCount - 1);

            switch (ComputedStyle.Alignment.GetAlignmentAlongAxis(primaryAxis))
            {
                case BoxAxisAlignment::Start:
                    primaryOffset += ComputedStyle.Padding.GetStartPaddingAlongAxis(primaryAxis);
                    break;
                case BoxAxisAlignment::Center:
                    primaryOffset += ComputedStyle.Padding.GetStartPaddingAlongAxis(primaryAxis);
                    primaryOffset += std::floor(
                        (
                            Size.GetValueAlongAxis(primaryAxis) -
                            ComputedStyle.Padding.GetTotalPaddingAlongAxis(primaryAxis) -
                            primaryContentSize
                        ) / 2.0f
                    );

                    // primaryOffset += std::max(
                    //     ComputedStyle.Padding.GetStartPaddingAlongAxis(primaryAxis),
                    //     std::floor(
                    //         (
                    //             Size.GetValueAlongAxis(primaryAxis) -
                    //             primaryContentSize -
                    //             ComputedStyle.Padding.GetEndPaddingAlongAxis(primaryAxis)
                    //         ) / 2.0f
                    //     )
                    // );
                    break;
                case BoxAxisAlignment::End:
                    primaryOffset += Size.GetValueAlongAxis(primaryAxis);
                    primaryOffset -= primaryContentSize;
                    primaryOffset -= ComputedStyle.Padding.GetEndPaddingAlongAxis(primaryAxis);
                    break;
            }

            for (auto& child : Children)
            {
                float secondaryOffset = Position.GetValueAlongAxis(secondaryAxis);

                switch (ComputedStyle.Alignment.GetAlignmentAlongAxis(secondaryAxis))
                {
                case BoxAxisAlignment::Start:
                    secondaryOffset += ComputedStyle.Padding.GetStartPaddingAlongAxis(secondaryAxis);
                    break;
                case BoxAxisAlignment::Center:
                    secondaryOffset += std::floor(
                        (
                            Size.GetValueAlongAxis(secondaryAxis) -
                            child->Size.GetValueAlongAxis(secondaryAxis)
                        ) / 2.0f
                    );
                    break;
                case BoxAxisAlignment::End:
                    secondaryOffset += Size.GetValueAlongAxis(secondaryAxis);
                    secondaryOffset -= ComputedStyle.Padding.GetEndPaddingAlongAxis(secondaryAxis);
                    secondaryOffset -= child->Size.GetValueAlongAxis(secondaryAxis);
                    break;
                }

                switch (child->ComputedStyle.Position.GetMode())
                {
                    case PositioningMode::Static:
                        child->Position.SetValueAlongAxis(primaryAxis, primaryOffset);
                        child->Position.SetValueAlongAxis(secondaryAxis, secondaryOffset);

                        primaryOffset += child->Size.GetValueAlongAxis(primaryAxis);
                        primaryOffset += ComputedStyle.Gap;
                        break;
                    case PositioningMode::Relative:
                        child->Position = Position + child->ComputedStyle.Position.GetOffset();
                        break;
                    default:
                        break;
                }

                child->ComputePosition();
            }
        }

        void Draw(RenderingContext& context) override
        {
            if (!ComputedStyle.Visibility)
            {
                return;
            }

            switch (ComputedStyle.Background.GetKind())
            {
                case BoxBackgroundKind::None:
                    break;
                case BoxBackgroundKind::Solid:
                    DrawSolidBackground(context);
                    break;
                case BoxBackgroundKind::Image:
                    DrawImageBackground(context);
                    break;
            }

            if (ComputedStyle.Border.IsSolid())
            {
                context.Color(ComputedStyle.Border.GetColor());
                context.StrokeRectangle(Position, Size, ComputedStyle.Border.GetWidth());
            }

            for (const auto& child : Children)
            {
                child->Draw(context);
            }
        }

        void AddChild(const std::shared_ptr<Element>& child)
        {
            Children.push_back(child);

            const std::shared_ptr<Screen>& screen = GetScreen();

            if (screen)
            {
                child->Mount(screen);
            }
        }

        std::shared_ptr<Element> GetChild(size_t index) const
        {
            if (index < 0 || index >= Children.size())
            {
                return nullptr;
            }

            return Children[index];
        }

        void RemoveChild(const std::shared_ptr<Element>& child)
        {
            if (GetScreen())
            {
                child->Unmount();
            }

            Children.erase(
                std::remove(Children.begin(), Children.end(), child),
                Children.end()
            );
        }

        void ClearChildren()
        {
            if (GetScreen())
            {
                for (const auto& child : Children)
                {
                    child->Unmount();
                }
            }

            Children.clear();
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

        void DrawSolidBackground(RenderingContext& context)
        {
            context.Color(ComputedStyle.Background.GetColor());
            context.FillRectangle(Position, Size);
        }

        void DrawImageBackground(RenderingContext& context)
        {
            auto bitmap = ComputedStyle.Background.GetBitmap().get();
            auto reference = ComputedStyle.BackgroundReference;

            Vec2 originalSize = Vec2(bitmap->GetWidth(), bitmap->GetHeight());

            Vec2 targetSize = originalSize;
            Vec2 targetPosition = Position;

            switch (ComputedStyle.BackgroundSize.GetMode())
            {
                case BoxBackgroundSizingMode::Fixed:
                    targetSize = Size;
                    break;
                case BoxBackgroundSizingMode::Contain:
                    float aspectRatio = originalSize.X / originalSize.Y;
                    float targetAspectRatio = Size.X / Size.Y;

                    if (aspectRatio > targetAspectRatio)
                    {
                        targetSize.X = Size.X;
                        targetSize.Y = std::floor(Size.X / aspectRatio);
                    }
                    else
                    {
                        targetSize.Y = Size.Y;
                        targetSize.X = std::floor(Size.Y * aspectRatio);
                    }
                    break;
            }

            switch (ComputedStyle.BackgroundPosition.GetMode())
            {
                case BoxBackgroundPositioningMode::Fixed:
                    targetPosition += ComputedStyle.BackgroundPosition.GetPosition();
                    break;
                case BoxBackgroundPositioningMode::Center:
                    targetPosition += Vec2(
                        std::floor((Size.X - targetSize.X) / 2.0f),
                        std::floor((Size.Y - targetSize.Y) / 2.0f)
                    );
                    targetPosition.Floor();
                    break;
            }

            m_BufferBitmap->Reallocate(targetSize.X, targetSize.Y);

            Bitmap::Scale(*bitmap, *m_BufferBitmap);

            if (reference.IsStatic())
            {
                context.Color(reference.GetStaticColor());
                context.FillRectangle(targetPosition, targetSize);
            }

            for (int x = 0; x < m_BufferBitmap->GetWidth(); x++)
            {
                for (int y = 0; y < m_BufferBitmap->GetHeight(); y++)
                {
                    const ColorRGBA& pixelColor = m_BufferBitmap->GetPixel(x, y);

                    switch (ComputedStyle.BackgroundReference.GetMode())
                    {
                        case BoxBackgroundTransparencyMode::Static:
                            if (pixelColor.A != 0.0f)
                            {
                                context.Color(pixelColor.CompositeOver(ComputedStyle.BackgroundReference.GetStaticColor()));
                                context.FillPoint(targetPosition + Vec2(x, y));
                            }
                            break;
                        case BoxBackgroundTransparencyMode::Checkerboard:
                            {
                                int checkerboardSize = ComputedStyle.BackgroundReference.GetCheckerboardSize();

                                int checkerboardX = x / checkerboardSize;
                                int checkerboardY = y / checkerboardSize;

                                ColorRGB checkerboardColor = (checkerboardX + checkerboardY) % 2 ?
                                    ComputedStyle.BackgroundReference.GetCheckerboardOddColor() :
                                    ComputedStyle.BackgroundReference.GetCheckerboardEvenColor();
                                
                                context.Color(pixelColor.CompositeOver(checkerboardColor));
                                context.FillPoint(targetPosition + Vec2(x, y));
                            }
                            break;
                    }
                }
            }
        }
    };
}