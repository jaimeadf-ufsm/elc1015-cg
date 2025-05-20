#pragma once

class Viewport
{
public:
    Viewport();
    Viewport(int width, int height);

    void SetWidth(int width);
    void SetHeight(int height);

    int GetWidth() const;
    int GetHeight() const;

private:
    int m_Width;
    int m_Height;
};