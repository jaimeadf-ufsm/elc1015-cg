#pragma once

#include <memory>

#include "Color.h"

/**
 * @file RenderingCommand.h
 * @brief Defines structures and classes for rendering commands used in a graphics rendering pipeline.
 * 
 * This file contains argument structures for various rendering commands, an enumeration for command types,
 * and the `RenderingCommand` class, which encapsulates a rendering command and its associated arguments.
 */

namespace yap
{
    /**
     * @brief Arguments for setting the rendering color.
     */
    struct ColorRenderingCommandArguments
    {
        float R;
        float G;
        float B;
    };

    /**
     * @brief Arguments for rendering a filled point.
     */
    struct FillPointRenderingCommandArguments
    {
        float X;
        float Y;
    };

    /**
     * @brief Arguments for rendering a stroked rectangle.
     */
    struct StrokeRectangleRenderingCommandArguments
    {
        float X;
        float Y;
        float Width;
        float Height;
        float StrokeWidth;
    };

    /**
     * @brief Arguments for rendering a filled rectangle.
     */
    struct FillRectangleRenderingCommandArguments
    {
        float X;
        float Y;
        float Width;
        float Height;
    };

    /**
     * @brief Arguments for beginning a polygon rendering.
     */
    struct BeginPolygonRenderingCommandArguments
    {
    };

    /**
     * @brief Arguments for adding a vertex to a polygon.
     */
    struct VertexCommandArguments
    {
        float X;
        float Y;
    };

    /**
     * @brief Arguments for rendering a stroked polygon.
     */
    struct StrokePolygonRenderingCommandArguments
    {
    };

    /**
     * @brief Arguments for rendering a filled polygon.
     */
    struct FillPolygonRenderingCommandArguments
    {
    };

    /**
     * @brief Arguments for rendering text.
     */
    struct TextRenderingCommandArguments
    {
        float X;
        float Y;
        const char* Text;
    };

    /**
     * @brief Enumeration of rendering command types.
     */
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

    /**
     * @brief Represents a rendering command with associated arguments.
     */
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