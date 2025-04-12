#pragma once

#include <memory>

#include "Vec2.h"
#include "Color.h"
#include "Bitmap.h"

/**
 * @file BoxBackground.h
 * @brief Defines classes and utilities for configuring background properties such as sizing, positioning, transparency, and kind.
 */

namespace yap
{
    /**
     * @enum BoxBackgroundSizingMode
     * @brief Defines the sizing mode for a box background.
     */
    enum class BoxBackgroundSizingMode
    {
        Fixed,
        Contain
    };

    /**
     * @class BoxBackgroundSizingRule
     * @brief Represents rules for sizing a box background.
     */
    class BoxBackgroundSizingRule
    {
    private:
        BoxBackgroundSizingMode m_Mode;
        Vec2 m_Size;

    public:
        BoxBackgroundSizingRule()
            : m_Mode(BoxBackgroundSizingMode::Fixed), m_Size(0.0f, 0.0f)
        {
        }

        BoxBackgroundSizingMode GetMode() const
        {
            return m_Mode;
        }

        const Vec2 &GetSize() const
        {
            return m_Size;
        }

        static BoxBackgroundSizingRule Fixed(const Vec2 &size)
        {
            return BoxBackgroundSizingRule(BoxBackgroundSizingMode::Fixed, size);
        }

        static BoxBackgroundSizingRule Contain()
        {
            return BoxBackgroundSizingRule(BoxBackgroundSizingMode::Contain, Vec2());
        }
    
    private:
        BoxBackgroundSizingRule(BoxBackgroundSizingMode mode, const Vec2 &size)
            : m_Mode(mode), m_Size(size)
        {
        }
    };

    /**
     * @enum BoxBackgroundPositioningMode
     * @brief Defines the positioning mode for a box background.
     */
    enum class BoxBackgroundPositioningMode
    {
        Fixed,
        Center
    };

    /**
     * @class BoxBackgroundPositioningRule
     * @brief Represents rules for positioning a box background.
     */
    class BoxBackgroundPositioningRule
    {
    private:
        BoxBackgroundPositioningMode m_Mode;
        Vec2 m_Position;
    
    public:
        BoxBackgroundPositioningRule()
            : m_Mode(BoxBackgroundPositioningMode::Fixed), m_Position(0.0f, 0.0f)
        {
        }

        BoxBackgroundPositioningMode GetMode() const
        {
            return m_Mode;
        }

        const Vec2 &GetPosition() const
        {
            return m_Position;
        }

        static BoxBackgroundPositioningRule Fixed(const Vec2 &position)
        {
            return BoxBackgroundPositioningRule(BoxBackgroundPositioningMode::Fixed, position);
        }

        static BoxBackgroundPositioningRule Center()
        {
            return BoxBackgroundPositioningRule(BoxBackgroundPositioningMode::Center, Vec2());
        }
    
    private:
        BoxBackgroundPositioningRule(BoxBackgroundPositioningMode mode, const Vec2 &position)
            : m_Mode(mode), m_Position(position)
        {
        }
    };

    /**
     * @enum BoxBackgroundTransparencyMode
     * @brief Defines the transparency mode for a box background.
     */
    enum class BoxBackgroundTransparencyMode
    {
        Static,
        Checkerboard
    };

    /**
     * @class BoxBackgroundTransparencyReference
     * @brief Represents transparency settings for a box background.
     */
    class BoxBackgroundTransparencyReference
    {
    private:
        BoxBackgroundTransparencyMode m_Mode;
        ColorRGB m_Color1;
        ColorRGB m_Color2;
        int m_Size;

    public:
        BoxBackgroundTransparencyReference()
            : m_Mode(BoxBackgroundTransparencyMode::Static), m_Color1(1.0f, 1.0f, 1.0f), m_Color2()
        {
        }

        BoxBackgroundTransparencyMode GetMode() const
        {
            return m_Mode;
        }

        const ColorRGB &GetStaticColor() const
        {
            return m_Color1;
        }

        const ColorRGB &GetCheckerboardOddColor() const
        {
            return m_Color1;
        }

        const ColorRGB &GetCheckerboardEvenColor() const
        {
            return m_Color2;
        }

        int GetCheckerboardSize() const
        {
            return m_Size;
        }

        bool IsStatic() const
        {
            return m_Mode == BoxBackgroundTransparencyMode::Static;
        }

        bool IsCheckerboard() const
        {
            return m_Mode == BoxBackgroundTransparencyMode::Checkerboard;
        }

        static BoxBackgroundTransparencyReference Static(const ColorRGB &color1)
        {
            return BoxBackgroundTransparencyReference(BoxBackgroundTransparencyMode::Static, color1, ColorRGB(), 0);
        }

        static BoxBackgroundTransparencyReference Checkerboard(const ColorRGB &evenColor, const ColorRGB &oddColor, int size = 8)
        {
            return BoxBackgroundTransparencyReference(BoxBackgroundTransparencyMode::Checkerboard, evenColor, oddColor, size);
        }

    private:
        BoxBackgroundTransparencyReference(BoxBackgroundTransparencyMode mode, const ColorRGB &color1, const ColorRGB &color2, int size)
            : m_Mode(mode), m_Color1(color1), m_Color2(color2), m_Size(size)
        {
        }
    };

    /**
     * @enum BoxBackgroundKind
     * @brief Defines the kind of box background.
     */
    enum class BoxBackgroundKind
    {
        None,
        Solid,
        Image
    };

    /**
     * @class BoxBackground
     * @brief Represents a box background with various properties.
     */
    class BoxBackground
    {
    private:
        BoxBackgroundKind m_Kind;
        ColorRGB m_Color;
        std::shared_ptr<const Bitmap> m_Bitmap;

    public:
        BoxBackground()
            : m_Kind(BoxBackgroundKind::None), m_Color(), m_Bitmap(nullptr)
        {
        }

        bool IsNone() const
        {
            return m_Kind == BoxBackgroundKind::None;
        }

        bool IsSolid() const
        {
            return m_Kind == BoxBackgroundKind::Solid;
        }

        bool IsImage() const
        {
            return m_Kind == BoxBackgroundKind::Image;
        }

        BoxBackgroundKind GetKind() const
        {
            return m_Kind;
        }

        const ColorRGB &GetColor() const
        {
            return m_Color;
        }

        std::shared_ptr<const Bitmap> GetBitmap() const
        {
            return m_Bitmap;
        }

        static BoxBackground Solid(const ColorRGB &color)
        {
            return BoxBackground(BoxBackgroundKind::Solid, color, nullptr);
        }

        static BoxBackground Image(const std::shared_ptr<const Bitmap> &bitmap)
        {
            return BoxBackground(BoxBackgroundKind::Image, ColorRGB(), bitmap);
        }

    private:
        BoxBackground(BoxBackgroundKind kind, const ColorRGB &color, const std::shared_ptr<const Bitmap> &bitmap)
            : m_Kind(kind), m_Color(color), m_Bitmap(bitmap) 
        {
        }
    };

}