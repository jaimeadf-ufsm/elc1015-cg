#pragma once

#include "ColorRGBA.h"

namespace yap
{
    struct ColorRenderingCommandArguments
    {
        const float R;
        const float G;
        const float B;
        const float A;
    };

    struct FillRectangleRenderingCommandArguments
    {
        const int X;
        const int Y;

        const int Width;
        const int Height;
    };

    enum class RenderingCommandKind
    {
        Color,
        FillRectangle
    };

    class RenderingCommand
    {
    public:
        const RenderingCommandKind Kind;
        const union 
        {
            ColorRenderingCommandArguments ColorArgs;
            FillRectangleRenderingCommandArguments FillRectangleArgs;
        };

        RenderingCommand(const ColorRenderingCommandArguments& args) : Kind(RenderingCommandKind::Color), ColorArgs(args) {}
        RenderingCommand(const FillRectangleRenderingCommandArguments& args) : Kind(RenderingCommandKind::FillRectangle), FillRectangleArgs(args) {}
    };
}