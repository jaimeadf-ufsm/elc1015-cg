#pragma once

#include <vector>

#include "gl_canvas2d.h"
#include "RenderingCommand.h"

/**
 * @file RenderingEngine.h
 * @brief Defines the RenderingEngine class, which processes and executes rendering commands
 *        for graphical output using the gl_canvas2d library.
 */

namespace yap
{
    /**
     * @class RenderingEngine
     * @brief A class responsible for executing a series of rendering commands to produce graphical output.
     */
    class RenderingEngine
    {
    private:
        std::vector<float> m_VerticesX; ///< Stores the X coordinates of vertices for polygon rendering.
        std::vector<float> m_VerticesY; ///< Stores the Y coordinates of vertices for polygon rendering.

    public:
        /**
         * @brief Executes a list of rendering commands.
         * @param commands A vector of RenderingCommand objects to be executed.
         */
        void ExecuteCommands(const std::vector<RenderingCommand>& commands)
        {
            for (const auto& command : commands)
            {
                ExecuteCommand(command);
            }
        }

    private:
        /**
         * @brief Executes a single rendering command based on its kind.
         * @param command The RenderingCommand to be executed.
         */
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

        /**
         * @brief Sets the current drawing color.
         * @param args The arguments containing RGB color values.
         */
        void ExecuteColorCommand(const ColorRenderingCommandArguments& args)
        {
            CV::color(args.R, args.G, args.B);
        }

        /**
         * @brief Draws a filled point at the specified coordinates.
         * @param args The arguments containing the X and Y coordinates of the point.
         */
        void ExecuteFillPointCommand(const FillPointRenderingCommandArguments& args)
        {
            CV::rectFill(args.X, args.Y, args.X + 1, args.Y + 1);
        }

        /**
         * @brief Draws a stroked rectangle with the specified dimensions and stroke width.
         * @param args The arguments containing the rectangle's position, dimensions, and stroke width.
         */
        void ExecuteStrokeRectangleCommand(const StrokeRectangleRenderingCommandArguments& args)
        {
            CV::rectFill(args.X, args.Y, args.X + args.Width, args.Y + args.StrokeWidth);
            CV::rectFill(args.X, args.Y + args.Height - args.StrokeWidth, args.X + args.Width, args.Y + args.Height);
            CV::rectFill(args.X, args.Y, args.X + args.StrokeWidth, args.Y + args.Height);
            CV::rectFill(args.X + args.Width - args.StrokeWidth, args.Y, args.X + args.Width, args.Y + args.Height);
        }

        /**
         * @brief Draws a filled rectangle with the specified dimensions.
         * @param args The arguments containing the rectangle's position and dimensions.
         */
        void ExecuteFillRectangleCommand(const FillRectangleRenderingCommandArguments& args)
        {
            CV::rectFill(args.X, args.Y, args.X + args.Width, args.Y + args.Height);
        }

        /**
         * @brief Begins a new polygon by clearing the current vertex lists.
         * @param args The arguments for the BeginPolygon command (unused).
         */
        void ExecuteBeginPolygonCommand(const BeginPolygonRenderingCommandArguments& args)
        {
            m_VerticesX.clear();
            m_VerticesY.clear();
        }

        /**
         * @brief Adds a vertex to the current polygon.
         * @param args The arguments containing the X and Y coordinates of the vertex.
         */
        void ExecuteVertexCommand(const VertexCommandArguments& args)
        {
            m_VerticesX.push_back(args.X);
            m_VerticesY.push_back(args.Y);
        }

        /**
         * @brief Draws the outline of the current polygon.
         * @param args The arguments for the StrokePolygon command (unused).
         */
        void ExecuteStrokePolygonCommand(const StrokePolygonRenderingCommandArguments& args)
        {
            if (m_VerticesX.size() < 2)
            {
                return;
            }

            CV::polygon(m_VerticesX.data(), m_VerticesY.data(), m_VerticesX.size());
        }

        /**
         * @brief Draws a filled polygon using the current vertices.
         * @param args The arguments for the FillPolygon command (unused).
         */
        void ExecuteFillPolygonCommand(const FillPolygonRenderingCommandArguments& args)
        {
            if (m_VerticesX.size() < 3)
            {
                return;
            }

            CV::polygonFill(m_VerticesX.data(), m_VerticesY.data(), m_VerticesX.size());
        }

        /**
         * @brief Draws text at the specified position.
         * @param args The arguments containing the position and text string.
         */
        void ExecuteTextCommand(const TextRenderingCommandArguments& args)
        {
            CV::text(args.X, args.Y, args.Text);
        }
    };
}