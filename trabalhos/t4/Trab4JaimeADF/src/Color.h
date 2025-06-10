/**
 * @file Color.h
 * @brief Color structures for RGB and RGBA color representation
 */

#pragma once

struct Vector3;
struct Vector4;

/**
 * @struct ColorRGB
 * @brief RGB color representation with floating-point components
 */
struct ColorRGB
{
    float R;
    float G;
    float B;

    ColorRGB();
    ColorRGB(int color);
    ColorRGB(int r, int g, int b);
    ColorRGB(float r, float g, float b);
    ColorRGB(const Vector3& vector);
};

/**
 * @struct ColorRGBA
 * @brief RGBA color representation with floating-point components and alpha channel
 */
struct ColorRGBA
{
    float R;
    float G;
    float B;
    float A;

    ColorRGBA();
    ColorRGBA(int color);
    ColorRGBA(int color, float alpha);
    ColorRGBA(int r, int g, int b, int a = 255);
    ColorRGBA(float r, float g, float b, float a = 1.0f);
    ColorRGBA(const ColorRGB& rgb, float alpha = 1.0f);
    ColorRGBA(const Vector4& vector);
};