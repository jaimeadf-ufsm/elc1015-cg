#pragma once

#include "ColorRGB.h"

namespace yap
{
    struct ColorRenderingCommandArguments
    {
        const float R;
        const float G;
        const float B;
    };

    struct FillRectangleRenderingCommandArguments
    {
        const int X;
        const int Y;

        const int Width;
        const int Height;
    };

    struct AddPolygonVertexRenderingCommandArguments
    {
        const int X;
        const int Y;
    };

    enum class RenderingCommandKind
    {
        Color,
        FillRectangle,
        BeginPolygon,
        AddPolygonVertex,
        FillPolygon
    };

    class RenderingCommand
    {
    private:
        RenderingCommandKind m_Kind;
        const union 
        {
            ColorRenderingCommandArguments m_ColorArgs;
            FillRectangleRenderingCommandArguments m_FillRectangleArgs;
        };

    public:
        RenderingCommand(const ColorRenderingCommandArguments& args) : 
            m_Kind(RenderingCommandKind::Color), m_ColorArgs(args) {}

        RenderingCommand(const FillRectangleRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::FillRectangle), m_FillRectangleArgs(args) {}

        RenderingCommandKind GetKind() const {
            return m_Kind;
        }

        const ColorRenderingCommandArguments& GetColorArgs() const {
            return m_ColorArgs;
        }

        const FillRectangleRenderingCommandArguments& GetFillRectangleArgs() const {
            return m_FillRectangleArgs;
        }
    };
}