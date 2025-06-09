#include "Graphics.h"

#include "gl_canvas2d.h"

std::vector<float> Graphics::s_VerticesX;
std::vector<float> Graphics::s_VerticesY;

void Graphics::StrokeLine(const ColorRGB& color, const PolyLine2D& polyline, float width)
{
    CV::color(color.R, color.G, color.B);

    float halfWidth = width / 2.0f;

    for (std::size_t i = 1; i < polyline.GetSize(); ++i)
    {
        Vector2 start = polyline.GetPoint(i - 1);
        Vector2 end = polyline.GetPoint(i);

        Vector2 direction = (end - start).Normalize();
        Vector2 perpendicular(-direction.Y, direction.X);

        Vector2 leftStart = start - perpendicular * halfWidth;
        Vector2 rightStart = start + perpendicular * halfWidth;

        Vector2 leftEnd = end - perpendicular * halfWidth;
        Vector2 rightEnd = end + perpendicular * halfWidth;

        s_VerticesX.clear();
        s_VerticesY.clear();

        s_VerticesX.push_back(leftStart.X);
        s_VerticesY.push_back(leftStart.Y);
        s_VerticesX.push_back(rightStart.X);
        s_VerticesY.push_back(rightStart.Y);
        s_VerticesX.push_back(rightEnd.X);
        s_VerticesY.push_back(rightEnd.Y);
        s_VerticesX.push_back(leftEnd.X);
        s_VerticesY.push_back(leftEnd.Y);
        s_VerticesX.push_back(leftStart.X);
        s_VerticesY.push_back(leftStart.Y);

        CV::polygonFill(s_VerticesX.data(), s_VerticesY.data(), static_cast<int>(s_VerticesX.size()));
    }
}

void Graphics::FillRectangle(const ColorRGB& color, const Vector2& position, const Vector2& size)
{
    CV::color(color.R, color.G, color.B);
    CV::rectFill(position.X, position.Y, position.X + size.X, position.Y + size.Y);
}

void Graphics::FillCircle(const ColorRGB& color, const Vector2& center, float radius)
{
    CV::color(color.R, color.G, color.B);
    CV::circleFill(center.X, center.Y, radius, 32);
}

void Graphics::DrawImage(const Image& image, const Vector2& position)
{
    // The teacher Pozzer has allowed us to use OpenGL directly for drawing images.
    glBegin(GL_POINTS);

    for (std::size_t y = 0; y < image.GetHeight(); ++y)
    {
        for (std::size_t x = 0; x < image.GetWidth(); ++x)
        {
            const ColorRGB& pixelColor = image.GetPixel(x, y);

            glColor3f(pixelColor.R, pixelColor.G, pixelColor.B);
            glVertex2f(position.X + x, position.Y + y);
        }
    }

    glEnd();

    // glBegin(GL_QUADS);

    // for (std::size_t y = 0; y < image.GetHeight(); ++y)
    // {
    //     for (std::size_t x = 0; x < image.GetWidth(); ++x)
    //     {
    //         const ColorRGB& pixelColor = image.GetPixel(x, y);

    //         glColor3f(pixelColor.R, pixelColor.G, pixelColor.B);
    //         glVertex2f(position.X + x, position.Y + y);
    //         glVertex2f(position.X + x + 1, position.Y + y);
    //         glVertex2f(position.X + x + 1, position.Y + y + 1);
    //         glVertex2f(position.X + x, position.Y + y + 1);
    //     }
    // }
}

void Graphics::DrawString(const ColorRGB& color, const Vector2& position, const std::string& text)
{
    CV::color(color.R, color.G, color.B);
    CV::text(position.X, position.Y, text.c_str());
}
