#include "gl_canvas2d.h"

#include "DrawingEngine.h"

void DrawingEngine::ExecuteCommands(const std::vector<DrawingCommand>& commands)
{
    for (const auto& command : commands)
    {
        switch (command.GetKind())
        {
            case DrawingCommandKind::Color:
                ExecuteColorCommand(command.GetColorArgs());
                break;
            case DrawingCommandKind::BeginPolygon:
                ExecuteBeginPolygonCommand(command.GetBeginPolygonArgs());
                break;
            case DrawingCommandKind::Vertex:
                ExecuteVertexCommand(command.GetVertexArgs());
                break;
            case DrawingCommandKind::StrokePolygon:
                ExecuteStrokePolygonCommand(command.GetStrokePolygonArgs());
                break;
            case DrawingCommandKind::FillPolygon:
                ExecuteFillPolygonCommand(command.GetFillPolygonArgs());
                break;
        }
    }
}

void DrawingEngine::ExecuteColorCommand(const ColorDrawingCommandArgs& args)
{
    CV::color(args.R, args.G, args.B);
}

void DrawingEngine::ExecuteBeginPolygonCommand(const BeginPolygonDrawingCommandArgs& args)
{
    m_VerticesX.clear();
    m_VerticesY.clear();
}

void DrawingEngine::ExecuteVertexCommand(const VertexDrawingCommandArgs& args)
{
    m_VerticesX.push_back(args.X);
    m_VerticesY.push_back(args.Y);
}

void DrawingEngine::ExecuteStrokePolygonCommand(const StrokePolygonDrawingCommandArgs& args)
{
    CV::polygon(m_VerticesX.data(), m_VerticesY.data(), m_VerticesX.size());
}

void DrawingEngine::ExecuteFillPolygonCommand(const FillPolygonDrawingCommandArgs& args)
{
    CV::polygonFill(m_VerticesX.data(), m_VerticesY.data(), m_VerticesX.size());
}
