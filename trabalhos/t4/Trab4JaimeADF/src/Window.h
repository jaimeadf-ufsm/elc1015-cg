/**
 * @file Window.h
 * @brief Window management class for dimensions and properties
 */

#pragma once

#include <cstddef>

/**
 * @class Window
 * @brief Static class for managing window dimensions and properties
 */
class Window
{
public:
    static std::size_t GetWidth();
    static std::size_t GetHeight();

    static void SetSize(std::size_t width, std::size_t height);

private:
    static std::size_t s_Width;
    static std::size_t s_Height;
};