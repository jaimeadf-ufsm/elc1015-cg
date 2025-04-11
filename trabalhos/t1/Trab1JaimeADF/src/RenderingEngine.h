#pragma once

#include <vector>

#include "gl_canvas2d.h"
#include "RenderingCommand.h"

namespace yap
{
    class RenderingEngine
    {
    private:
        std::vector<float> m_VerticesX;
        std::vector<float> m_VerticesY;

    public:
        void ExecuteCommands(const std::vector<RenderingCommand>& commands)
        {
            for (const auto& command : commands)
            {
                ExecuteCommand(command);
            }
        }
    
    private:
        void ExecuteCommand(const RenderingCommand& command)
        {
            switch (command.GetKind())
            {
                case RenderingCommandKind::Color:
                    ExecuteColorCommand(command.GetColorArgs());
                    break;
                case RenderingCommandKind::FillPoint:
                    ExecuteFillPointCommand(command.GetFillPointArgs());
                    break;
                case RenderingCommandKind::StrokeRectangle:
                    ExecuteStrokeRectangleCommand(command.GetStrokeRectangleArgs());
                    break;
                case RenderingCommandKind::FillRectangle:
                    ExecuteFillRectangleCommand(command.GetFillRectangleArgs());
                    break;
                case RenderingCommandKind::BeginPolygon:
                    ExecuteBeginPolygonCommand(command.GetBeginPolygonArgs());
                    break;
                case RenderingCommandKind::Vertex:
                    ExecuteVertexCommand(command.GetVertexArgs());
                    break;
                case RenderingCommandKind::StrokePolygon:
                    ExecuteStrokePolygonCommand(command.GetStrokePolygonArgs());
                    break;
                case RenderingCommandKind::FillPolygon:
                    ExecuteFillPolygonCommand(command.GetFillPolygonArgs());
                    break;
                case RenderingCommandKind::Text:
                    ExecuteTextCommand(command.GetTextArgs());
                    break;
            }
        }

        void ExecuteColorCommand(const ColorRenderingCommandArguments& args)
        {
            CV::color(args.R, args.G, args.B);

            // printf(
            //    "Color(R = %.2f, G = %.2f, B = %.2f)\n",
            //    args.R,
            //    args.G,
            //    args.B
            // );
        }

        void ExecuteFillPointCommand(const FillPointRenderingCommandArguments& args)
        {
            // CV::point(args.X, args.Y);
            CV::rectFill(args.X, args.Y, args.X + 1, args.Y + 1);

            // printf(
            //    "FillPoint(X = %f, Y = %f)\n",
            //    args.X,
            //    args.Y
            // );
        }

        void ExecuteStrokeRectangleCommand(const StrokeRectangleRenderingCommandArguments& args)
        {
            // CV::rect(args.X, args.Y, args.X + args.Width, args.Y + args.Height);
            CV::rectFill(args.X, args.Y, args.X + args.Width, args.Y + args.StrokeWidth);
            CV::rectFill(args.X, args.Y + args.Height - args.StrokeWidth, args.X + args.Width, args.Y + args.Height);
            CV::rectFill(args.X, args.Y, args.X + args.StrokeWidth, args.Y + args.Height);
            CV::rectFill(args.X + args.Width - args.StrokeWidth, args.Y, args.X + args.Width, args.Y + args.Height);

            // printf(
            //    "StrokeRectangle(X = %f, Y = %f, Width = %f, Height = %f)\n",
            //    args.X,
            //    args.Y,
            //    args.Width,
            //    args.Height
            // );
        }

        void ExecuteFillRectangleCommand(const FillRectangleRenderingCommandArguments& args)
        {
            CV::rectFill(args.X, args.Y, args.X + args.Width, args.Y + args.Height);

            // printf(
            //    "FillRectangle(X = %f, Y = %f, Width = %f, Height = %f)\n",
            //    args.X,
            //    args.Y,
            //    args.Width,
            //    args.Height
            // );
        }

        void ExecuteBeginPolygonCommand(const BeginPolygonRenderingCommandArguments& args)
        {
            m_VerticesX.clear();
            m_VerticesY.clear();

            // printf("BeginPolygon()\n");
        }

        void ExecuteVertexCommand(const VertexCommandArguments& args)
        {
            m_VerticesX.push_back(args.X);
            m_VerticesY.push_back(args.Y);

            // printf(
            //    "Vertex(X = %f, Y = %f)\n",
            //    args.X,
            //    args.Y
            // );
        }

        void ExecuteStrokePolygonCommand(const StrokePolygonRenderingCommandArguments& args)
        {
            if (m_VerticesX.size() < 2)
            {
                return;
            }

            CV::polygon(m_VerticesX.data(), m_VerticesY.data(), m_VerticesX.size());

            // printf("StrokePolygon()\n");
        }

        void ExecuteFillPolygonCommand(const FillPolygonRenderingCommandArguments& args)
        {
            if (m_VerticesX.size() < 3)
            {
                return;
            }

            CV::polygonFill(m_VerticesX.data(), m_VerticesY.data(), m_VerticesX.size());

            // printf("FillPolygon()\n");
        }

        void ExecuteTextCommand(const TextRenderingCommandArguments& args)
        {
            CV::text(args.X, args.Y, args.Text);

            // printf(
            //    "Text(X = %f, Y = %f, Text = %s)\n",
            //    args.X,
            //    args.Y,
            //    args.Text
            // );
        }
    };
}