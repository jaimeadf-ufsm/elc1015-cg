#include "Window.h"

std::size_t Window::s_Width = 1280;
std::size_t Window::s_Height = 720;

void Window::SetSize(std::size_t width, std::size_t height)
{
    s_Width = width;
    s_Height = height;
}

std::size_t Window::GetWidth()
{
    return s_Width;
}

std::size_t Window::GetHeight()
{
    return s_Height;
}