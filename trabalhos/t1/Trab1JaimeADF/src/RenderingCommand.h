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
        float R; ///< Red component of the color.
        float G; ///< Green component of the color.
        float B; ///< Blue component of the color.
    };

    /**
     * @brief Arguments for rendering a filled point.
     */
    struct FillPointRenderingCommandArguments
    {
        float X; ///< X-coordinate of the point.
        float Y; ///< Y-coordinate of the point.
    };

    /**
     * @brief Arguments for rendering a stroked rectangle.
     */
    struct StrokeRectangleRenderingCommandArguments
    {
        float X; ///< X-coordinate of the rectangle's top-left corner.
        float Y; ///< Y-coordinate of the rectangle's top-left corner.
        float Width; ///< Width of the rectangle.
        float Height; ///< Height of the rectangle.
        float StrokeWidth; ///< Width of the rectangle's stroke.
    };

    /**
     * @brief Arguments for rendering a filled rectangle.
     */
    struct FillRectangleRenderingCommandArguments
    {
        float X; ///< X-coordinate of the rectangle's top-left corner.
        float Y; ///< Y-coordinate of the rectangle's top-left corner.
        float Width; ///< Width of the rectangle.
        float Height; ///< Height of the rectangle.
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
        float X; ///< X-coordinate of the vertex.
        float Y; ///< Y-coordinate of the vertex.
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
        float X; ///< X-coordinate of the text's starting position.
        float Y; ///< Y-coordinate of the text's starting position.
        const char* Text; ///< The text to render.
    };

    /**
     * @brief Enumeration of rendering command types.
     */
    enum class RenderingCommandKind
    {
        Color, ///< Command to set the rendering color.
        FillPoint, ///< Command to render a filled point.
        StrokeRectangle, ///< Command to render a stroked rectangle.
        FillRectangle, ///< Command to render a filled rectangle.
        BeginPolygon, ///< Command to begin a polygon rendering.
        Vertex, ///< Command to add a vertex to a polygon.
        StrokePolygon, ///< Command to render a stroked polygon.
        FillPolygon, ///< Command to render a filled polygon.
        Text ///< Command to render text.
    };

    /**
     * @brief Represents a rendering command with associated arguments.
     */
    class RenderingCommand
    {
    private:
        RenderingCommandKind m_Kind; ///< The type of the rendering command.
        const union 
        {
            ColorRenderingCommandArguments m_ColorArgs; ///< Arguments for a color command.
            FillPointRenderingCommandArguments m_FillPointArgs; ///< Arguments for a fill point command.
            FillRectangleRenderingCommandArguments m_FillRectangleArgs; ///< Arguments for a fill rectangle command.
            StrokeRectangleRenderingCommandArguments m_StrokeRectangleArgs; ///< Arguments for a stroke rectangle command.
            BeginPolygonRenderingCommandArguments m_BeginPolygonArgs; ///< Arguments for a begin polygon command.
            VertexCommandArguments m_VertexArgs; ///< Arguments for a vertex command.
            StrokePolygonRenderingCommandArguments m_StrokePolygonArgs; ///< Arguments for a stroke polygon command.
            FillPolygonRenderingCommandArguments m_FillPolygonArgs; ///< Arguments for a fill polygon command.
            TextRenderingCommandArguments m_TextArgs; ///< Arguments for a text command.
        };

    public:
        /**
         * @brief Constructs a rendering command with color arguments.
         * @param args The color arguments.
         */
        RenderingCommand(const ColorRenderingCommandArguments& args) : 
            m_Kind(RenderingCommandKind::Color), m_ColorArgs(args) {}
        
        /**
         * @brief Constructs a rendering command with fill point arguments.
         * @param args The fill point arguments.
         */
        RenderingCommand(const FillPointRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::FillPoint), m_FillPointArgs(args) {}

        /**
         * @brief Constructs a rendering command with stroke rectangle arguments.
         * @param args The stroke rectangle arguments.
         */
        RenderingCommand(const StrokeRectangleRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::StrokeRectangle), m_StrokeRectangleArgs(args) {}

        /**
         * @brief Constructs a rendering command with fill rectangle arguments.
         * @param args The fill rectangle arguments.
         */
        RenderingCommand(const FillRectangleRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::FillRectangle), m_FillRectangleArgs(args) {}
        
        /**
         * @brief Constructs a rendering command with begin polygon arguments.
         * @param args The begin polygon arguments.
         */
        RenderingCommand(const BeginPolygonRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::BeginPolygon), m_BeginPolygonArgs(args) {}
        
        /**
         * @brief Constructs a rendering command with vertex arguments.
         * @param args The vertex arguments.
         */
        RenderingCommand(const VertexCommandArguments& args) :
            m_Kind(RenderingCommandKind::Vertex), m_VertexArgs(args) {}
        
        /**
         * @brief Constructs a rendering command with stroke polygon arguments.
         * @param args The stroke polygon arguments.
         */
        RenderingCommand(const StrokePolygonRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::StrokePolygon), m_StrokePolygonArgs(args) {}
        
        /**
         * @brief Constructs a rendering command with fill polygon arguments.
         * @param args The fill polygon arguments.
         */
        RenderingCommand(const FillPolygonRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::FillPolygon), m_FillPolygonArgs(args) {}
        
        /**
         * @brief Constructs a rendering command with text arguments.
         * @param args The text arguments.
         */
        RenderingCommand(const TextRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::Text), m_TextArgs(args) {}

        /**
         * @brief Gets the type of the rendering command.
         * @return The rendering command type.
         */
        RenderingCommandKind GetKind() const {
            return m_Kind;
        }

        /**
         * @brief Gets the color arguments.
         * @return The color arguments.
         */
        const ColorRenderingCommandArguments& GetColorArgs() const {
            return m_ColorArgs;
        }

        /**
         * @brief Gets the fill point arguments.
         * @return The fill point arguments.
         */
        const FillPointRenderingCommandArguments& GetFillPointArgs() const {
            return m_FillPointArgs;
        }

        /**
         * @brief Gets the stroke rectangle arguments.
         * @return The stroke rectangle arguments.
         */
        const StrokeRectangleRenderingCommandArguments& GetStrokeRectangleArgs() const {
            return m_StrokeRectangleArgs;
        }

        /**
         * @brief Gets the fill rectangle arguments.
         * @return The fill rectangle arguments.
         */
        const FillRectangleRenderingCommandArguments& GetFillRectangleArgs() const {
            return m_FillRectangleArgs;
        }

        /**
         * @brief Gets the begin polygon arguments.
         * @return The begin polygon arguments.
         */
        const BeginPolygonRenderingCommandArguments& GetBeginPolygonArgs() const {
            return m_BeginPolygonArgs;
        }

        /**
         * @brief Gets the vertex arguments.
         * @return The vertex arguments.
         */
        const VertexCommandArguments& GetVertexArgs() const {
            return m_VertexArgs;
        }

        /**
         * @brief Gets the stroke polygon arguments.
         * @return The stroke polygon arguments.
         */
        const StrokePolygonRenderingCommandArguments& GetStrokePolygonArgs() const {
            return m_StrokePolygonArgs;
        }

        /**
         * @brief Gets the fill polygon arguments.
         * @return The fill polygon arguments.
         */
        const FillPolygonRenderingCommandArguments& GetFillPolygonArgs() const {
            return m_FillPolygonArgs;
        }

        /**
         * @brief Gets the text arguments.
         * @return The text arguments.
         */
        const TextRenderingCommandArguments& GetTextArgs() const {
            return m_TextArgs;
        }
    };
}