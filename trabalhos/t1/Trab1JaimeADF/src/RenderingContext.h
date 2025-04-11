#pragma once

#include <vector>

#include "Vec2.h"
#include "RenderingCommand.h"

#include "gl_canvas2d.h"

/**
 * @file RenderingContext.h
 * @brief Defines the RenderingContext class, which provides an interface for managing and issuing rendering commands.
 * 
 * The RenderingContext class allows users to define various rendering operations such as drawing shapes, text, and polygons.
 * Commands are stored in a queue and can be retrieved or cleared as needed.
 */

namespace yap
{
    /**
     * @class RenderingContext
     * @brief Manages a collection of rendering commands and provides methods to define rendering operations.
     */
    class RenderingContext
    {
    private:
        std::vector<RenderingCommand> m_Commands; ///< Stores the list of rendering commands.

    public:
        /**
         * @brief Constructs an empty RenderingContext.
         */
        RenderingContext() {}

        /**
         * @brief Sets the current drawing color.
         * @param color The RGB color to set.
         */
        void Color(const ColorRGB& color)
        {
            ColorRenderingCommandArguments args = {
                .R = color.R,
                .G = color.G,
                .B = color.B
            };

            m_Commands.emplace_back(args);
        }

        /**
         * @brief Fills a single point at the specified position.
         * @param point The position of the point to fill.
         */
        void FillPoint(const Vec2 &point)
        {
            FillPointRenderingCommandArguments args = {
                .X = point.X,
                .Y = point.Y
            };

            m_Commands.emplace_back(args);
        }

        /**
         * @brief Draws the outline of a rectangle.
         * @param position The top-left corner of the rectangle.
         * @param size The width and height of the rectangle.
         * @param strokeWidth The width of the rectangle's outline. Default is 1.0f.
         */
        void StrokeRectangle(const Vec2 &position, const Vec2 &size, float strokeWidth = 1.0f)
        {
            StrokeRectangleRenderingCommandArguments args = {
                .X = position.X,
                .Y = position.Y,
                .Width = size.X,
                .Height = size.Y,
                .StrokeWidth = strokeWidth
            };

            m_Commands.emplace_back(args);
        }

        /**
         * @brief Fills a rectangle with the current color.
         * @param position The top-left corner of the rectangle.
         * @param size The width and height of the rectangle.
         */
        void FillRectangle(const Vec2 &position, const Vec2 &size)
        {
            FillRectangleRenderingCommandArguments args = {
                .X = position.X,
                .Y = position.Y,
                .Width = size.X,
                .Height = size.Y
            };

            m_Commands.emplace_back(args);
        }

        /**
         * @brief Begins the definition of a polygon.
         */
        void BeginPolygon()
        {
            BeginPolygonRenderingCommandArguments args = {};

            m_Commands.emplace_back(args);
        }

        /**
         * @brief Adds a vertex to the current polygon.
         * @param vertex The position of the vertex.
         */
        void Vertex(const Vec2 &vertex)
        {
            VertexCommandArguments args = {
                .X = vertex.X,
                .Y = vertex.Y
            };

            m_Commands.emplace_back(args);
        }

        /**
         * @brief Draws the outline of the current polygon.
         */
        void StrokePolygon()
        {
            StrokePolygonRenderingCommandArguments args = {};

            m_Commands.emplace_back(args);
        }

        /**
         * @brief Fills the current polygon with the current color.
         */
        void FillPolygon()
        {
            FillPolygonRenderingCommandArguments args = {};

            m_Commands.emplace_back(args);
        }

        /**
         * @brief Draws text at the specified position.
         * @param position The position where the text will be drawn.
         * @param text The text to draw.
         */
        void Text(const Vec2 &position, const char *text)
        {
            TextRenderingCommandArguments args = {
                .X = position.X,
                .Y = position.Y,
                .Text = text
            };

            m_Commands.emplace_back(args);
        }

        /**
         * @brief Draws a line between two points with the specified stroke width.
         * @param start The starting point of the line.
         * @param end The ending point of the line.
         * @param strokeWidth The width of the line. Default is 1.0f.
         */
        void Line(const Vec2 &start, const Vec2 &end, float strokeWidth = 1.0f)
        {
            Vec2 direction = Vec2::Normalize(end - start);
            Vec2 normal = Vec2(-direction.Y, direction.X);

            Vec2 offset = normal * (strokeWidth / 2.0f);

            BeginPolygon();
            Vertex(start + offset);
            Vertex(start - offset);
            Vertex(end - offset);
            Vertex(end + offset);
            FillPolygon();
        }

        /**
         * @brief Retrieves the list of rendering commands.
         * @return A constant reference to the vector of rendering commands.
         */
        const std::vector<RenderingCommand>& GetCommands() const
        {
            return m_Commands;
        }

        /**
         * @brief Clears all rendering commands from the context.
         */
        void ClearCommands()
        {
            m_Commands.clear();
        }
    };
}