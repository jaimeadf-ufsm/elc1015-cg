#include "DrawingCommand.h"

DrawingCommand::DrawingCommand(ColorDrawingCommandArgs args)
{
    Kind = DrawingCommandKind::Color;
    ColorArgs = args;
}

DrawingCommand::DrawingCommand(FillRectangleDrawingCommandArgs args)
{
    Kind = DrawingCommandKind::FillRectangle;
    FillRectangleArgs = args;
}

DrawingCommand::DrawingCommand(BeginPolygonDrawingCommandArgs args)
{
    Kind = DrawingCommandKind::BeginPolygon;
    BeginPolygonArgs = args;
}

DrawingCommand::DrawingCommand(VertexDrawingCommandArgs args)
{
    Kind = DrawingCommandKind::Vertex;
    VertexArgs = args;
}

DrawingCommand::DrawingCommand(StrokePolygonDrawingCommandArgs args)
{
    Kind = DrawingCommandKind::StrokePolygon;
    StrokePolygonArgs = args;
}

DrawingCommand::DrawingCommand(FillPolygonDrawingCommandArgs args)
{
    Kind = DrawingCommandKind::FillPolygon;
    FillPolygonArgs = args;
}

DrawingCommand::DrawingCommand(TextDrawingCommandArgs args)
{
    Kind = DrawingCommandKind::Text;
    TextArgs = args;
}

DrawingCommandKind DrawingCommand::GetKind() const
{
    return Kind;
}

ColorDrawingCommandArgs DrawingCommand::GetColorArgs() const
{
    return ColorArgs;
}

TextDrawingCommandArgs DrawingCommand::GetTextArgs() const
{
    return TextArgs;
}

FillRectangleDrawingCommandArgs DrawingCommand::GetFillRectangleArgs() const
{
    return FillRectangleArgs;
}

BeginPolygonDrawingCommandArgs DrawingCommand::GetBeginPolygonArgs() const
{
    return BeginPolygonArgs;
}

VertexDrawingCommandArgs DrawingCommand::GetVertexArgs() const
{
    return VertexArgs;
}

StrokePolygonDrawingCommandArgs DrawingCommand::GetStrokePolygonArgs() const
{
    return StrokePolygonArgs;
}

FillPolygonDrawingCommandArgs DrawingCommand::GetFillPolygonArgs() const
{
    return FillPolygonArgs;
}
