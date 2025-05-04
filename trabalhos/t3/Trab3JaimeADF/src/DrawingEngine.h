#pragma once

#include <vector>

#include "DrawingCommand.h"

class DrawingEngine
{
private:
    std::vector<float> m_VerticesX;
    std::vector<float> m_VerticesY;

public:
    void ExecuteCommands(const std::vector<DrawingCommand>& commands);

private:
    void ExecuteColorCommand(const ColorDrawingCommandArgs& args);
    void ExecuteFillRectangleCommand(const FillRectangleDrawingCommandArgs& args);
    void ExecuteBeginPolygonCommand(const BeginPolygonDrawingCommandArgs& args);
    void ExecuteVertexCommand(const VertexDrawingCommandArgs& args);
    void ExecuteStrokePolygonCommand(const StrokePolygonDrawingCommandArgs& args);
    void ExecuteFillPolygonCommand(const FillPolygonDrawingCommandArgs& args);
};