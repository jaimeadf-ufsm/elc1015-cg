#pragma once

#include <memory>

#include "Color.h"

namespace yap
{
    struct ColorRenderingCommandArguments
    {
        float R;
        float G;
        float B;
    };

    struct FillPointRenderingCommandArguments
    {
        float X;
        float Y;
    };

    struct StrokeRectangleRenderingCommandArguments
    {
        float X;
        float Y;
        float Width;
        float Height;
        float StrokeWidth;
    };

    struct FillRectangleRenderingCommandArguments
    {
        float X;
        float Y;
        float Width;
        float Height;
    };

    struct BeginPolygonRenderingCommandArguments
    {
    };

    struct VertexCommandArguments
    {
        float X;
        float Y;
    };

    struct StrokePolygonRenderingCommandArguments
    {
    };

    struct FillPolygonRenderingCommandArguments
    {
    };

    struct TextRenderingCommandArguments
    {
        float X;
        float Y;
        const char* Text;
    };

    enum class RenderingCommandKind
    {
        Color,
        FillPoint,
        StrokeRectangle,
        FillRectangle,
        BeginPolygon,
        Vertex,
        StrokePolygon,
        FillPolygon,
        Text
    };

    class RenderingCommand
    {
    private:
        RenderingCommandKind m_Kind;
        const union 
        {
            ColorRenderingCommandArguments m_ColorArgs;
            FillPointRenderingCommandArguments m_FillPointArgs;
            FillRectangleRenderingCommandArguments m_FillRectangleArgs;
            StrokeRectangleRenderingCommandArguments m_StrokeRectangleArgs;
            BeginPolygonRenderingCommandArguments m_BeginPolygonArgs;
            VertexCommandArguments m_VertexArgs;
            StrokePolygonRenderingCommandArguments m_StrokePolygonArgs;
            FillPolygonRenderingCommandArguments m_FillPolygonArgs;
            TextRenderingCommandArguments m_TextArgs;
        };

    public:
        RenderingCommand(const ColorRenderingCommandArguments& args) : 
            m_Kind(RenderingCommandKind::Color), m_ColorArgs(args) {}
        
        RenderingCommand(const FillPointRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::FillPoint), m_FillPointArgs(args) {}

        RenderingCommand(const StrokeRectangleRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::StrokeRectangle), m_StrokeRectangleArgs(args) {}

        RenderingCommand(const FillRectangleRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::FillRectangle), m_FillRectangleArgs(args) {}
        
        RenderingCommand(const BeginPolygonRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::BeginPolygon), m_BeginPolygonArgs(args) {}
        
        RenderingCommand(const VertexCommandArguments& args) :
            m_Kind(RenderingCommandKind::Vertex), m_VertexArgs(args) {}
        
        RenderingCommand(const StrokePolygonRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::StrokePolygon), m_StrokePolygonArgs(args) {}
        
        RenderingCommand(const FillPolygonRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::FillPolygon), m_FillPolygonArgs(args) {}
        
        RenderingCommand(const TextRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::Text), m_TextArgs(args) {}

        RenderingCommandKind GetKind() const {
            return m_Kind;
        }

        const ColorRenderingCommandArguments& GetColorArgs() const {
            return m_ColorArgs;
        }

        const FillPointRenderingCommandArguments& GetFillPointArgs() const {
            return m_FillPointArgs;
        }

        const StrokeRectangleRenderingCommandArguments& GetStrokeRectangleArgs() const {
            return m_StrokeRectangleArgs;
        }

        const FillRectangleRenderingCommandArguments& GetFillRectangleArgs() const {
            return m_FillRectangleArgs;
        }

        const BeginPolygonRenderingCommandArguments& GetBeginPolygonArgs() const {
            return m_BeginPolygonArgs;
        }

        const VertexCommandArguments& GetVertexArgs() const {
            return m_VertexArgs;
        }

        const StrokePolygonRenderingCommandArguments& GetStrokePolygonArgs() const {
            return m_StrokePolygonArgs;
        }

        const FillPolygonRenderingCommandArguments& GetFillPolygonArgs() const {
            return m_FillPolygonArgs;
        }

        const TextRenderingCommandArguments& GetTextArgs() const {
            return m_TextArgs;
        }
    };
}