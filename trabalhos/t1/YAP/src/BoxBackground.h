#pragma once

#include <memory>

#include "Vec2.h"
#include "Color.h"
#include "Bitmap.h"

namespace yap
{
    enum class BoxBackgroundTransparencyMode
    {
        Static,
        Checkerboard
    };

    class BoxBackgroundTransparencyReference
    {
    private:
        BoxBackgroundTransparencyMode m_Mode;
        ColorRGB m_Color1;
        ColorRGB m_Color2;

    public:
        BoxBackgroundTransparencyReference()
            : m_Mode(BoxBackgroundTransparencyMode::Static), m_Color1(1.0f, 1.0f, 1.0f), m_Color2()
        {
        }

        ColorRGB GetReferenceColor(const Vec2 &size, const Vec2 &position) const
        {
            switch (m_Mode)
            {
            case BoxBackgroundTransparencyMode::Static:
                return m_Color1;
            case BoxBackgroundTransparencyMode::Checkerboard:
                return ((int(position.X) + int(position.Y)) % 2 == 0) ? m_Color1 : m_Color2;
            default:
                return m_Color1; // Fallback to color1 if mode is unknown
            }
        }

        BoxBackgroundTransparencyMode GetMode() const
        {
            return m_Mode;
        }

        const ColorRGB &GetColor1() const
        {
            return m_Color1;
        }

        const ColorRGB &GetColor2() const
        {
            return m_Color2;
        }

        static BoxBackgroundTransparencyReference Static(const ColorRGB &color1)
        {
            return BoxBackgroundTransparencyReference(BoxBackgroundTransparencyMode::Static, color1, ColorRGB());
        }

        static BoxBackgroundTransparencyReference Checkerboard(const ColorRGB &evenColor, const ColorRGB &oddColor)
        {
            return BoxBackgroundTransparencyReference(BoxBackgroundTransparencyMode::Checkerboard, evenColor, oddColor);
        }

    private:
        BoxBackgroundTransparencyReference(BoxBackgroundTransparencyMode mode, const ColorRGB &color1, const ColorRGB &color2)
            : m_Mode(mode), m_Color1(color1), m_Color2(color2)
        {
        }
    };

    enum class BoxBackgroundKind
    {
        None,
        Solid,
        Image
    };

    class BoxBackground
    {
    private:
        BoxBackgroundKind m_Kind;
        ColorRGB m_Color;
        std::shared_ptr<Bitmap> m_Bitmap;
        BoxBackgroundTransparencyReference m_TransparencyReference;

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

        const std::shared_ptr<Bitmap> &GetBitmap() const
        {
            return m_Bitmap;
        }

        const BoxBackgroundTransparencyReference &GetTransparencyReference() const
        {
            return m_TransparencyReference;
        }

        static BoxBackground Solid(const ColorRGB &color)
        {
            return BoxBackground(BoxBackgroundKind::Solid, color, nullptr);
        }

        static BoxBackground Image(
            const std::shared_ptr<Bitmap> &bitmap,
            const BoxBackgroundTransparencyReference &reference = BoxBackgroundTransparencyReference())
        {
            return BoxBackground(BoxBackgroundKind::Image, ColorRGB(), bitmap, reference);
        }

    private:
        BoxBackground(
            BoxBackgroundKind kind,
            const ColorRGB &color,
            const std::shared_ptr<Bitmap> &bitmap,
            const BoxBackgroundTransparencyReference &reference = BoxBackgroundTransparencyReference())
            : m_Kind(kind), m_Color(color), m_Bitmap(bitmap), m_TransparencyReference(reference)
        {
        }
    };

}