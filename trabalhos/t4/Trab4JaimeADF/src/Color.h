#pragma once

struct Color
{
    float R;
    float G;
    float B;

    Color();
    Color(int color);
    Color(int r, int g, int b);
    Color(float r, float g, float b);
};