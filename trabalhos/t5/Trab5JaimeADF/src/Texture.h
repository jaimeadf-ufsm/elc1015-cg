/**
 * @file Texture.h
 * @brief Texture class for loading and managing OpenGL textures
 */

#pragma once

#include <GL/glut.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>
#include "Vector.h"

/**
 * @class Texture
 * @brief Handles texture loading, binding, and management for OpenGL rendering
 */
class Texture
{
public:
    GLuint TextureID;

    Texture(GLuint textureID = 0) : TextureID(textureID)
    {
        if (TextureID == 0)
        {
            glGenTextures(1, &TextureID);
        }
    }

    ~Texture()
    {
        if (TextureID != 0)
        {
            glDeleteTextures(1, &TextureID);
        }
    }

    void Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, TextureID);
    }

    void Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void LoadCheckerboard(int width, int height, Vector3 color1, Vector3 color2, int checkSize = 8)
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

        LoadFromPixels(width, height, data.data());
    }

    bool LoadFromBMP(const char* filename)
    {
        std::ifstream file(filename, std::ios::binary);

        if (!file.is_open())
        {
            std::cerr << "Error: Could not open BMP file: " << filename << std::endl;
            return false;
        }

        #pragma pack(push, 1)
        struct BMPHeader {
            char signature[2];        // "BM"
            uint32_t fileSize;        // File size in bytes
            uint32_t reserved;        // Reserved (unused)
            uint32_t dataOffset;      // Offset to image data
        };

        struct BMPInfoHeader {
            uint32_t headerSize;      // Size of this header
            int32_t width;            // Image width
            int32_t height;           // Image height
            uint16_t planes;          // Number of color planes
            uint16_t bitsPerPixel;    // Bits per pixel
            uint32_t compression;     // Compression type
            uint32_t imageSize;       // Image size in bytes
            int32_t xPixelsPerMeter;  // Horizontal resolution
            int32_t yPixelsPerMeter;  // Vertical resolution
            uint32_t colorsUsed;      // Number of colors used
            uint32_t colorsImportant; // Number of important colors
        };
        #pragma pack(pop)

        BMPHeader header;
        BMPInfoHeader infoHeader;

        file.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));
        
        if (header.signature[0] != 'B' || header.signature[1] != 'M')
        {
            std::cerr << "Error: Invalid BMP file format" << std::endl;
            file.close();
            return false;
        }

        file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));

        if (infoHeader.bitsPerPixel != 24)
        {
            std::cerr << "Error: Only 24-bit BMP files are supported" << std::endl;
            file.close();
            return false;
        }

        if (infoHeader.compression != 0)
        {
            std::cerr << "Error: Compressed BMP files are not supported" << std::endl;
            file.close();
            return false;
        }

        int width = infoHeader.width;
        int height = abs(infoHeader.height);
        bool isTopDown = infoHeader.height < 0;

        int rowPadding = (4 - (width * 3) % 4) % 4;
        int rowSize = width * 3 + rowPadding;

        std::vector<unsigned char> bmpData(rowSize * height);
        std::vector<unsigned char> rgbData(width * height * 3);

        file.seekg(header.dataOffset);

        file.read(reinterpret_cast<char*>(bmpData.data()), bmpData.size());
        file.close();

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int srcY = isTopDown ? y : (height - 1 - y);
                int srcIndex = srcY * rowSize + x * 3;
                int dstIndex = (y * width + x) * 3;

                rgbData[dstIndex] = bmpData[srcIndex + 2];
                rgbData[dstIndex + 1] = bmpData[srcIndex + 1];
                rgbData[dstIndex + 2] = bmpData[srcIndex];
            }
        }

        LoadFromPixels(width, height, rgbData.data());

        return true;
    }

    void LoadFromPixels(int width, int height, const unsigned char* data)
    {
        Bind();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        Unbind();
    }
};
