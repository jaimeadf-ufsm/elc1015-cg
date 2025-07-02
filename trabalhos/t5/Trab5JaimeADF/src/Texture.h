#pragma once

#include <GL/glut.h>
#include <vector>
#include "Vector.h"

class Texture
{
public:
    GLuint TextureID;

    Texture() : TextureID(0) {}

    ~Texture()
    {
        if (TextureID != 0)
        {
            glDeleteTextures(1, &TextureID);
        }
    }

    void CreateCheckerboard(int width, int height, Vector3 color1, Vector3 color2, int checkSize = 8)
    {
        std::vector<unsigned char> data(width * height * 3);

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                bool checker = ((x / checkSize) + (y / checkSize)) % 2 == 0;
                Vector3 color = checker ? color1 : color2;

                int index = (y * width + x) * 3;
                data[index] = (unsigned char)(color.X * 255);
                data[index + 1] = (unsigned char)(color.Y * 255);
                data[index + 2] = (unsigned char)(color.Z * 255);
            }
        }

        glGenTextures(1, &TextureID);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    void Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, TextureID);
    }

    void Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    GLuint m_TextureID;
};
