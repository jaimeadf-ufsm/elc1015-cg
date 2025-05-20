#pragma once

enum class DrawingCommandKind
{
    Color,
    Text,
    FillRectangle,
    BeginPolygon,
    Vertex,
    StrokePolygon,
    FillPolygon
};

struct ColorDrawingCommandArgs
{
    float R;
    float G;
    float B;
};

struct TextDrawingCommandArgs
{
    float X;
    float Y;
    char Text[256];
};

struct FillRectangleDrawingCommandArgs
{
    float X;
    float Y;
    float Width;
    float Height;
};

struct BeginPolygonDrawingCommandArgs
{
    // No additional data needed for this command
};

struct VertexDrawingCommandArgs
{
    float X;
    float Y;
};

struct StrokePolygonDrawingCommandArgs
{
    // No additional data needed for this command
};

struct FillPolygonDrawingCommandArgs
{
    // No additional data needed for this command
};

class DrawingCommand
{
private:
    DrawingCommandKind Kind;

    union
    {
        ColorDrawingCommandArgs ColorArgs;
        TextDrawingCommandArgs TextArgs;
        FillRectangleDrawingCommandArgs FillRectangleArgs;
        BeginPolygonDrawingCommandArgs BeginPolygonArgs;
        VertexDrawingCommandArgs VertexArgs;
        StrokePolygonDrawingCommandArgs StrokePolygonArgs;
        FillPolygonDrawingCommandArgs FillPolygonArgs;
    };

public:
    DrawingCommand(ColorDrawingCommandArgs args);
    DrawingCommand(TextDrawingCommandArgs args);
    DrawingCommand(FillRectangleDrawingCommandArgs args);
    DrawingCommand(BeginPolygonDrawingCommandArgs args);
    DrawingCommand(VertexDrawingCommandArgs args);
    DrawingCommand(StrokePolygonDrawingCommandArgs args);
    DrawingCommand(FillPolygonDrawingCommandArgs args);

    DrawingCommandKind GetKind() const;

    ColorDrawingCommandArgs GetColorArgs() const;
    TextDrawingCommandArgs GetTextArgs() const;
    FillRectangleDrawingCommandArgs GetFillRectangleArgs() const;
    BeginPolygonDrawingCommandArgs GetBeginPolygonArgs() const;
    VertexDrawingCommandArgs GetVertexArgs() const;
    StrokePolygonDrawingCommandArgs GetStrokePolygonArgs() const;
    FillPolygonDrawingCommandArgs GetFillPolygonArgs() const;
};