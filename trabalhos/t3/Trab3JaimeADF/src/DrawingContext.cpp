#include "DrawingContext.h"

void DrawingContext::Color(ColorRGB color)
{
    ColorDrawingCommandArgs args = {
        .R = color.R,
        .G = color.G,
        .B = color.B
    };

    m_Commands.emplace_back(args);
}

void DrawingContext::FillRectangle(Vector2 position, Vector2 size)
{
    FillRectangleDrawingCommandArgs args = {
        .X = position.X,
        .Y = position.Y,
        .Width = size.X,
        .Height = size.Y
    };

    m_Commands.emplace_back(args);
}

void DrawingContext::BeginPolygon()
{
    BeginPolygonDrawingCommandArgs args = {};

    m_Commands.emplace_back(args);
}

void DrawingContext::Vertex(Vector2 position)
{
    VertexDrawingCommandArgs args = {
        .X = position.X,
        .Y = position.Y
    };

    m_Commands.emplace_back(args);
}

void DrawingContext::StrokePolygon()
{
    StrokePolygonDrawingCommandArgs args = {};

    m_Commands.emplace_back(args);
}

void DrawingContext::FillPolygon()
{
    FillPolygonDrawingCommandArgs args = {};

    m_Commands.emplace_back(args);
}

void DrawingContext::ClearCommands()
{
    m_Commands.clear();
}

const std::vector<DrawingCommand>& DrawingContext::GetCommands() const
{
    return m_Commands;
}
