#pragma once

#include "Vec2.h"
#include "Layer.h"
#include "ColorPalette.h"

/**
 * @file Brush.h
 * @brief Defines the Brush base class and its derived PencilBrush class for drawing on layers.
 * 
 * This file contains the Brush base class, which provides a common interface for brushes,
 * and the PencilBrush class, which implements a specific type of brush with customizable shapes.
 * Brushes are used to apply colors to layers and perform strokes.
 */

namespace yap
{
    /**
     * @class Brush
     * @brief Abstract base class for brushes used to draw on layers.
     * 
     * The Brush class provides a common interface for applying colors to layers
     * and performing strokes. Derived classes must implement the `Apply` and `Stroke` methods.
     */
    class Brush
    {
    private:
        std::shared_ptr<ColorPalette> m_ColorPalette; ///< The color palette used by the brush.
        float m_Size = 8; ///< The size of the brush.

    public:
        /**
         * @brief Constructs a Brush with the specified color palette.
         * @param colorPalette Shared pointer to the color palette.
         */
        Brush(const std::shared_ptr<ColorPalette>& colorPalette) : m_ColorPalette(colorPalette)
        {
        }

        /**
         * @brief Applies the brush at a specific position on a layer.
         * @param layer The layer to apply the brush to.
         * @param position The position where the brush is applied.
         */
        virtual void Apply(std::shared_ptr<Layer> layer, const Vec2& position) = 0;

        /**
         * @brief Performs a stroke from a start position to an end position on a layer.
         * @param layer The layer to apply the stroke to.
         * @param start The starting position of the stroke.
         * @param end The ending position of the stroke.
         */
        virtual void Stroke(std::shared_ptr<Layer> layer, const Vec2& start, const Vec2& end) = 0;

        /**
         * @brief Sets the size of the brush.
         * @param size The new size of the brush.
         */
        void SetSize(float size)
        {
            m_Size = size;
        }

        /**
         * @brief Gets the size of the brush.
         * @return The size of the brush.
         */
        float GetSize() const
        {
            return m_Size;
        }

        /**
         * @brief Gets the color palette used by the brush.
         * @return Shared pointer to the color palette.
         */
        std::shared_ptr<ColorPalette> GetColorPalette() const
        {
            return m_ColorPalette;
        }
    };

    /**
     * @enum PencilShape
     * @brief Enum representing the shape of the PencilBrush.
     */
    enum class PencilShape
    {
        Circle, ///< Circular shape.
        Square, ///< Square shape.
        Triangle ///< Triangular shape.
    };

    /**
     * @class PencilBrush
     * @brief A brush that applies colors in different shapes (circle, square, triangle).
     * 
     * The PencilBrush class extends the Brush class and provides functionality
     * for applying colors in customizable shapes. It supports circle, square, and triangle shapes.
     */
    class PencilBrush : public Brush
    {
    private:
        PencilShape m_Shape = PencilShape::Circle; ///< The shape of the pencil brush.

    public:
        /**
         * @brief Constructs a PencilBrush with the specified color palette.
         * @param colorPalette Shared pointer to the color palette.
         */
        PencilBrush(std::shared_ptr<ColorPalette> colorPalette) : Brush(colorPalette)
        {
        }

        /**
         * @brief Applies the pencil brush at a specific position on a layer.
         * @param layer The layer to apply the brush to.
         * @param position The position where the brush is applied.
         */
        void Apply(std::shared_ptr<Layer> layer, const Vec2& position) override
        {
            ColorRGBA color = GetColorPalette()->GetGlobalColor();

            float size = GetSize();
            int halfSize = static_cast<int>(std::ceil(size / 2.0f));

            for (int y = -halfSize; y <= halfSize; ++y)
            {
                for (int x = -halfSize; x <= halfSize; ++x)
                {
                    if (IsInsideShape(x, y, size))
                    {
                        int pixelX = static_cast<int>(position.X) + x;
                        int pixelY = static_cast<int>(position.Y) + y;

                        layer->SetPixel(pixelX, pixelY, color);
                    }
                }
            }
        }

        /**
         * @brief Performs a stroke from a start position to an end position on a layer.
         * @param layer The layer to apply the stroke to.
         * @param start The starting position of the stroke.
         * @param end The ending position of the stroke.
         */
        void Stroke(std::shared_ptr<Layer> layer, const Vec2& start, const Vec2& end) override
        {
            Vec2 direction = end - start;
            float length = direction.Length();

            direction.Normalize();

            for (float t = 0; t <= length; t += 1.0f)
            {
                Vec2 position = start + direction * t;
                Apply(layer, position);
            }
        }

        /**
         * @brief Sets the shape of the pencil brush.
         * @param shape The new shape of the pencil brush.
         */
        void SetShape(PencilShape shape)
        {
            m_Shape = shape;
        }

        /**
         * @brief Gets the current shape of the pencil brush.
         * @return The current shape of the pencil brush.
         */
        PencilShape GetShape() const
        {
            return m_Shape;
        }

    private:
        /**
         * @brief Checks if a point is inside the current shape of the brush.
         * @param x The x-coordinate of the point.
         * @param y The y-coordinate of the point.
         * @param size The size of the brush.
         * @return True if the point is inside the shape, false otherwise.
         */
        bool IsInsideShape(int x, int y, float size)
        {
            switch (m_Shape)
            {
                case PencilShape::Circle:
                    return IsInsideCircle(x, y, size);
                case PencilShape::Square:
                    return IsInsideSquare(x, y, size);
                case PencilShape::Triangle:
                    return IsInsideTriangle(x, y, size);
                default:
                    return false;
            }
        }

        /**
         * @brief Checks if a point is inside a circle.
         * @param x The x-coordinate of the point.
         * @param y The y-coordinate of the point.
         * @param size The size of the circle.
         * @return True if the point is inside the circle, false otherwise.
         */
        bool IsInsideCircle(int x, int y, float size)
        {
            return x * x + y * y <= (size * size) / 4.0f;
        }

        /**
         * @brief Checks if a point is inside a square.
         * @param x The x-coordinate of the point.
         * @param y The y-coordinate of the point.
         * @param size The size of the square.
         * @return True if the point is inside the square, false otherwise.
         */
        bool IsInsideSquare(int x, int y, float size)
        {
            return std::abs(x) <= size / 2.0f && std::abs(y) <= size / 2.0f;
        }

        /**
         * @brief Checks if a point is inside a triangle.
         * @param x The x-coordinate of the point.
         * @param y The y-coordinate of the point.
         * @param size The size of the triangle.
         * @return True if the point is inside the triangle, false otherwise.
         */
        bool IsInsideTriangle(int x, int y, float size)
        {
            return y >= 0 && y <= size && x >= -y && x <= y;
        }
    };
}