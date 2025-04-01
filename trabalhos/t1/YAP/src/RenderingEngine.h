#pragma once

#include <vector>

#include "gl_canvas2d.h"
#include "RenderingCommand.h"

namespace yap
{
    class RenderingEngine
    {
    public:
        void ProcessCommands(const std::vector<RenderingCommand>& commands)
        {
            for (const auto& command : commands)
            {
                ProcessCommand(command);
            }
        }
    
    private:
        void ProcessCommand(const RenderingCommand& command)
        {
            switch (command.GetKind())
            {
                case RenderingCommandKind::Color:
                    ProcessColorCommand(command.GetColorArgs());
                    break;
                case RenderingCommandKind::FillRectangle:
                    ProcessFillRectangleCommand(command.GetFillRectangleArgs());
                    break;
            }
        }

        void ProcessColorCommand(const ColorRenderingCommandArguments& args)
        {
            CV::color(args.R, args.G, args.B);

            // printf(
            //    "Color(R = %.2f, G = %.2f, B = %.2f)\n",
            //    args.R,
            //    args.G,
            //    args.B
            // );
        }

        void ProcessFillRectangleCommand(const FillRectangleRenderingCommandArguments& args)
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
    };
}