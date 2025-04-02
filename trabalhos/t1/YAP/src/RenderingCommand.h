#pragma once

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

    struct FillRectangleRenderingCommandArguments
    {
        float X;
        float Y;
        float Width;
        float Height;
    };

    enum class RenderingCommandKind
    {
        Color,
        FillPoint,
        FillRectangle
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
        };

    public:
        RenderingCommand(const ColorRenderingCommandArguments& args) : 
            m_Kind(RenderingCommandKind::Color), m_ColorArgs(args) {}
        
        RenderingCommand(const FillPointRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::FillPoint), m_FillPointArgs(args) {}

        RenderingCommand(const FillRectangleRenderingCommandArguments& args) :
            m_Kind(RenderingCommandKind::FillRectangle), m_FillRectangleArgs(args) {}

        RenderingCommandKind GetKind() const {
            return m_Kind;
        }

        const ColorRenderingCommandArguments& GetColorArgs() const {
            return m_ColorArgs;
        }

        const FillPointRenderingCommandArguments& GetFillPointArgs() const {
            return m_FillPointArgs;
        }

        const FillRectangleRenderingCommandArguments& GetFillRectangleArgs() const {
            return m_FillRectangleArgs;
        }
    };
}