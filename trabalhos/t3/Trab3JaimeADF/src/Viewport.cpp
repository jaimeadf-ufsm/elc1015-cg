#include "Viewport.h"

Viewport::Viewport() : Viewport(0, 0)
{

}

Viewport::Viewport(int width, int height) : m_Width(width), m_Height(height)
{

}

void Viewport::SetWidth(int width)
{
    m_Width = width;
}

void Viewport::SetHeight(int height)
{
    m_Height = height;
}

int Viewport::GetWidth() const
{
    return m_Width;
}

int Viewport::GetHeight() const
{
    return m_Height;
}