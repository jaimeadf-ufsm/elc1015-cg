#pragma once

#include <vector>

#include "Vec2.h"
#include "RenderingCommand.h"

namespace yap
{
    class RenderingContext
    {
    private:
        std::vector<RenderingCommand> m_Commands;
    
    public:
        RenderingContext() {}

        void Color(const ColorRGB& color)
        {
            ColorRenderingCommandArguments args = {
                .R = color.R,
                .G = color.G,
                .B = color.B
            };

            m_Commands.emplace_back(args);
        }

        void FillPoint(const Vec2 &point)
        {
            FillPointRenderingCommandArguments args = {
                .X = point.X,
                .Y = point.Y
            };

            m_Commands.emplace_back(args);
        }

        void FillRectangle(const Vec2 &position, const Vec2 &size)
        {
            FillRectangleRenderingCommandArguments args = {
                .X = position.X,
                .Y = position.Y,
                .Width = size.X,
                .Height = size.Y
            };

            m_Commands.emplace_back(args);
        }

        const std::vector<RenderingCommand>& GetCommands() const
        {
            return m_Commands;
        }

        void ClearCommands()
        {
            m_Commands.clear();
        }
    };
}