#pragma once

#include <vector>

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
                color.R,
                color.G,
                color.B
            };

            m_Commands.emplace_back(args);
        }

        void FillRectangle(float x, float y, float width, float height)
        {
            FillRectangleRenderingCommandArguments args = {
                x,
                y,
                width,
                height
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