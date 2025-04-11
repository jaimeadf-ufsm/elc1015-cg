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
        Fixed,   ///< Fixed size.
        Contain  ///< Contain within a specific area.
    };

    /**
     * @class BoxBackgroundSizingRule
     * @brief Represents rules for sizing a box background.
     */
    class BoxBackgroundSizingRule
    {
    private:
        BoxBackgroundSizingMode m_Mode; ///< The sizing mode.
        Vec2 m_Size; ///< The size of the box background.

    public:
        /**
         * @brief Default constructor. Initializes with Fixed mode and size (0, 0).
         */
        BoxBackgroundSizingRule()
            : m_Mode(BoxBackgroundSizingMode::Fixed), m_Size(0.0f, 0.0f)
        {
        }

        /**
         * @brief Gets the sizing mode.
         * @return The current sizing mode.
         */
        BoxBackgroundSizingMode GetMode() const
        {
            return m_Mode;
        }

        /**
         * @brief Gets the size of the box background.
         * @return A reference to the size vector.
         */
        const Vec2 &GetSize() const
        {
            return m_Size;
        }

        /**
         * @brief Creates a sizing rule with Fixed mode and a specific size.
         * @param size The size of the box background.
         * @return A new BoxBackgroundSizingRule instance.
         */
        static BoxBackgroundSizingRule Fixed(const Vec2 &size)
        {
            return BoxBackgroundSizingRule(BoxBackgroundSizingMode::Fixed, size);
        }

        /**
         * @brief Creates a sizing rule with Contain mode.
         * @return A new BoxBackgroundSizingRule instance.
         */
        static BoxBackgroundSizingRule Contain()
        {
            return BoxBackgroundSizingRule(BoxBackgroundSizingMode::Contain, Vec2());
        }

    private:
        /**
         * @brief Private constructor for internal use.
         * @param mode The sizing mode.
         * @param size The size of the box background.
         */
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
        Fixed,  ///< Fixed position.
        Center  ///< Centered position.
    };

    /**
     * @class BoxBackgroundPositioningRule
     * @brief Represents rules for positioning a box background.
     */
    class BoxBackgroundPositioningRule
    {
    private:
        BoxBackgroundPositioningMode m_Mode; ///< The positioning mode.
        Vec2 m_Position; ///< The position of the box background.

    public:
        /**
         * @brief Default constructor. Initializes with Fixed mode and position (0, 0).
         */
        BoxBackgroundPositioningRule()
            : m_Mode(BoxBackgroundPositioningMode::Fixed), m_Position(0.0f, 0.0f)
        {
        }

        /**
         * @brief Gets the positioning mode.
         * @return The current positioning mode.
         */
        BoxBackgroundPositioningMode GetMode() const
        {
            return m_Mode;
        }

        /**
         * @brief Gets the position of the box background.
         * @return A reference to the position vector.
         */
        const Vec2 &GetPosition() const
        {
            return m_Position;
        }

        /**
         * @brief Creates a positioning rule with Fixed mode and a specific position.
         * @param position The position of the box background.
         * @return A new BoxBackgroundPositioningRule instance.
         */
        static BoxBackgroundPositioningRule Fixed(const Vec2 &position)
        {
            return BoxBackgroundPositioningRule(BoxBackgroundPositioningMode::Fixed, position);
        }

        /**
         * @brief Creates a positioning rule with Center mode.
         * @return A new BoxBackgroundPositioningRule instance.
         */
        static BoxBackgroundPositioningRule Center()
        {
            return BoxBackgroundPositioningRule(BoxBackgroundPositioningMode::Center, Vec2());
        }

    private:
        /**
         * @brief Private constructor for internal use.
         * @param mode The positioning mode.
         * @param position The position of the box background.
         */
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
        Static,       ///< Static transparency.
        Checkerboard  ///< Checkerboard transparency pattern.
    };

    /**
     * @class BoxBackgroundTransparencyReference
     * @brief Represents transparency settings for a box background.
     */
    class BoxBackgroundTransparencyReference
    {
    private:
        BoxBackgroundTransparencyMode m_Mode; ///< The transparency mode.
        ColorRGB m_Color1; ///< The primary color for transparency.
        ColorRGB m_Color2; ///< The secondary color for checkerboard mode.
        int m_Size; ///< The size of the checkerboard pattern.

    public:
        /**
         * @brief Default constructor. Initializes with Static mode and white color.
         */
        BoxBackgroundTransparencyReference()
            : m_Mode(BoxBackgroundTransparencyMode::Static), m_Color1(1.0f, 1.0f, 1.0f), m_Color2()
        {
        }

        /**
         * @brief Gets the transparency mode.
         * @return The current transparency mode.
         */
        BoxBackgroundTransparencyMode GetMode() const
        {
            return m_Mode;
        }

        /**
         * @brief Gets the static color for transparency.
         * @return A reference to the static color.
         */
        const ColorRGB &GetStaticColor() const
        {
            return m_Color1;
        }

        /**
         * @brief Gets the odd color for checkerboard mode.
         * @return A reference to the odd color.
         */
        const ColorRGB &GetCheckerboardOddColor() const
        {
            return m_Color1;
        }

        /**
         * @brief Gets the even color for checkerboard mode.
         * @return A reference to the even color.
         */
        const ColorRGB &GetCheckerboardEvenColor() const
        {
            return m_Color2;
        }

        /**
         * @brief Gets the size of the checkerboard pattern.
         * @return The size of the checkerboard pattern.
         */
        int GetCheckerboardSize() const
        {
            return m_Size;
        }

        /**
         * @brief Checks if the transparency mode is Static.
         * @return True if Static, false otherwise.
         */
        bool IsStatic() const
        {
            return m_Mode == BoxBackgroundTransparencyMode::Static;
        }

        /**
         * @brief Checks if the transparency mode is Checkerboard.
         * @return True if Checkerboard, false otherwise.
         */
        bool IsCheckerboard() const
        {
            return m_Mode == BoxBackgroundTransparencyMode::Checkerboard;
        }

        /**
         * @brief Creates a transparency reference with Static mode.
         * @param color1 The static color.
         * @return A new BoxBackgroundTransparencyReference instance.
         */
        static BoxBackgroundTransparencyReference Static(const ColorRGB &color1)
        {
            return BoxBackgroundTransparencyReference(BoxBackgroundTransparencyMode::Static, color1, ColorRGB(), 0);
        }

        /**
         * @brief Creates a transparency reference with Checkerboard mode.
         * @param evenColor The even color for the checkerboard.
         * @param oddColor The odd color for the checkerboard.
         * @param size The size of the checkerboard pattern (default is 8).
         * @return A new BoxBackgroundTransparencyReference instance.
         */
        static BoxBackgroundTransparencyReference Checkerboard(const ColorRGB &evenColor, const ColorRGB &oddColor, int size = 8)
        {
            return BoxBackgroundTransparencyReference(BoxBackgroundTransparencyMode::Checkerboard, evenColor, oddColor, size);
        }

    private:
        /**
         * @brief Private constructor for internal use.
         * @param mode The transparency mode.
         * @param color1 The primary color.
         * @param color2 The secondary color.
         * @param size The size of the checkerboard pattern.
         */
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
        None,   ///< No background.
        Solid,  ///< Solid color background.
        Image   ///< Image background.
    };

    /**
     * @class BoxBackground
     * @brief Represents a box background with various properties.
     */
    class BoxBackground
    {
    private:
        BoxBackgroundKind m_Kind; ///< The kind of background.
        ColorRGB m_Color; ///< The color of the background (if Solid).
        std::shared_ptr<const Bitmap> m_Bitmap; ///< The bitmap for the background (if Image).

    public:
        /**
         * @brief Default constructor. Initializes with None kind.
         */
        BoxBackground()
            : m_Kind(BoxBackgroundKind::None), m_Color(), m_Bitmap(nullptr)
        {
        }

        /**
         * @brief Checks if the background kind is None.
         * @return True if None, false otherwise.
         */
        bool IsNone() const
        {
            return m_Kind == BoxBackgroundKind::None;
        }

        /**
         * @brief Checks if the background kind is Solid.
         * @return True if Solid, false otherwise.
         */
        bool IsSolid() const
        {
            return m_Kind == BoxBackgroundKind::Solid;
        }

        /**
         * @brief Checks if the background kind is Image.
         * @return True if Image, false otherwise.
         */
        bool IsImage() const
        {
            return m_Kind == BoxBackgroundKind::Image;
        }

        /**
         * @brief Gets the kind of the background.
         * @return The current background kind.
         */
        BoxBackgroundKind GetKind() const
        {
            return m_Kind;
        }

        /**
         * @brief Gets the color of the background (if Solid).
         * @return A reference to the color.
         */
        const ColorRGB &GetColor() const
        {
            return m_Color;
        }

        /**
         * @brief Gets the bitmap of the background (if Image).
         * @return A shared pointer to the bitmap.
         */
        std::shared_ptr<const Bitmap> GetBitmap() const
        {
            return m_Bitmap;
        }

        /**
         * @brief Creates a solid color background.
         * @param color The color of the background.
         * @return A new BoxBackground instance.
         */
        static BoxBackground Solid(const ColorRGB &color)
        {
            return BoxBackground(BoxBackgroundKind::Solid, color, nullptr);
        }

        /**
         * @brief Creates an image background.
         * @param bitmap A shared pointer to the bitmap.
         * @return A new BoxBackground instance.
         */
        static BoxBackground Image(const std::shared_ptr<const Bitmap> &bitmap)
        {
            return BoxBackground(BoxBackgroundKind::Image, ColorRGB(), bitmap);
        }

    private:
        /**
         * @brief Private constructor for internal use.
         * @param kind The kind of background.
         * @param color The color of the background.
         * @param bitmap A shared pointer to the bitmap.
         */
        BoxBackground(BoxBackgroundKind kind, const ColorRGB &color, const std::shared_ptr<const Bitmap> &bitmap)
            : m_Kind(kind), m_Color(color), m_Bitmap(bitmap) 
        {
        }
    };

}