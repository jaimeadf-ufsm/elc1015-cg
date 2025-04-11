#pragma once

#include <vector>

#include "Vec2.h"
#include "RenderingCommand.h"

#include "gl_canvas2d.h"

namespace yap
{
    class RenderingContext
    {
    private:
        std::vector<RenderingCommand> m_Commands;
    
    public:
        RenderingContext() {}

        void Color(const ColorRGB& color)
        {
            ColorRenderingCommandArguments args = {
                .R = color.R,
                .G = color.G,
                .B = color.B
            };

            m_Commands.emplace_back(args);
        }

        void FillPoint(const Vec2 &point)
        {
            FillPointRenderingCommandArguments args = {
                .X = point.X,
                .Y = point.Y
            };

            m_Commands.emplace_back(args);
        }

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

        void BeginPolygon()
        {
            BeginPolygonRenderingCommandArguments args = {};

            m_Commands.emplace_back(args);
        }

        void Vertex(const Vec2 &vertex)
        {
            VertexCommandArguments args = {
                .X = vertex.X,
                .Y = vertex.Y
            };

            m_Commands.emplace_back(args);
        }

        void StrokePolygon()
        {
            StrokePolygonRenderingCommandArguments args = {};

            m_Commands.emplace_back(args);
        }

        void FillPolygon()
        {
            FillPolygonRenderingCommandArguments args = {};

            m_Commands.emplace_back(args);
        }

        void Text(const Vec2 &position, const char *text)
        {
            TextRenderingCommandArguments args = {
                .X = position.X,
                .Y = position.Y,
                .Text = text
            };

            m_Commands.emplace_back(args);
        }

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

        const std::vector<RenderingCommand>& GetCommands() const
        {
            return m_Commands;
        }

        void ClearCommands()
        {
            m_Commands.clear();
        }
    };
}