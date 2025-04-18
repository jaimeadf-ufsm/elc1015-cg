#pragma once

#include <fstream>
#include <cstdint>

#include "Bitmap.h"

/**
 * @file BMP.h
 * @brief Provides functionality for loading and saving BMP image files.
 * 
 * This file defines the BMP class, which includes methods for reading BMP files
 * into a Bitmap object and saving Bitmap objects as BMP files. It supports both
 * 24-bit and 32-bit BMP formats, with optional alpha channel handling.
 */

namespace yap
{
    /**
     * @class BMP
     * @brief Handles loading and saving BMP image files.
     * 
     * The BMP class provides static methods to load BMP files into Bitmap objects
     * and save Bitmap objects as BMP files. It supports 24-bit and 32-bit BMP formats,
     * with optional alpha channel support for 32-bit BMPs.
     */
    class BMP
    {
    public:
        static Bitmap Load(const std::string& path)
        {
            std::ifstream file(path, std::ios::binary);

            if (!file)
            {
                throw std::runtime_error("Unable to open BMP file");
            }

            Header header;
            InfoHeader infoHeader;

            file.read(reinterpret_cast<char*>(&header.Type), sizeof(header.Type));
            file.read(reinterpret_cast<char*>(&header.Size), sizeof(header.Size));
            file.read(reinterpret_cast<char*>(&header.Reserved1), sizeof(header.Reserved1));
            file.read(reinterpret_cast<char*>(&header.Reserved2), sizeof(header.Reserved2));
            file.read(reinterpret_cast<char*>(&header.Offset), sizeof(header.Offset));

            if (header.Type != 0x4D42)
            {
                throw std::runtime_error("Invalid BMP file format");
            }

            file.read(reinterpret_cast<char*>(&infoHeader.Size), sizeof(infoHeader.Size));
            file.read(reinterpret_cast<char*>(&infoHeader.Width), sizeof(infoHeader.Width));
            file.read(reinterpret_cast<char*>(&infoHeader.Height), sizeof(infoHeader.Height));
            file.read(reinterpret_cast<char*>(&infoHeader.Planes), sizeof(infoHeader.Planes));
            file.read(reinterpret_cast<char*>(&infoHeader.BitsPerPixel), sizeof(infoHeader.BitsPerPixel));
            file.read(reinterpret_cast<char*>(&infoHeader.Compression), sizeof(infoHeader.Compression));
            file.read(reinterpret_cast<char*>(&infoHeader.ImageSize), sizeof(infoHeader.ImageSize));
            file.read(reinterpret_cast<char*>(&infoHeader.XPixelsPerMeter), sizeof(infoHeader.XPixelsPerMeter));
            file.read(reinterpret_cast<char*>(&infoHeader.YPixelsPerMeter), sizeof(infoHeader.YPixelsPerMeter));
            file.read(reinterpret_cast<char*>(&infoHeader.ColorUsed), sizeof(infoHeader.ColorUsed));
            file.read(reinterpret_cast<char*>(&infoHeader.ColorImportant), sizeof(infoHeader.ColorImportant));

            if (infoHeader.BitsPerPixel != 24 && infoHeader.BitsPerPixel != 32)
            {
                throw std::runtime_error("Only 24-bit and 32-bit BMP files are supported");
            }

            if (infoHeader.Planes != 1)
            {
                throw std::runtime_error("Invalid number of planes in BMP file");
            }

            if (infoHeader.Width <= 0 || infoHeader.Height <= 0)
            {
                throw std::runtime_error("Invalid BMP dimensions");
            }

            if (infoHeader.BitsPerPixel == 24 && infoHeader.Compression != 0)
            {
                throw std::runtime_error("24-bit compressed BMP files are not supported");
            }

            if (infoHeader.BitsPerPixel == 32 && infoHeader.Compression != 0)
            {
                if (infoHeader.Compression == 3)
                {
                    file.read(reinterpret_cast<char*>(&infoHeader.RedMask), sizeof(infoHeader.RedMask));
                    file.read(reinterpret_cast<char*>(&infoHeader.GreenMask), sizeof(infoHeader.GreenMask));
                    file.read(reinterpret_cast<char*>(&infoHeader.BlueMask), sizeof(infoHeader.BlueMask));
                    file.read(reinterpret_cast<char*>(&infoHeader.AlphaMask), sizeof(infoHeader.AlphaMask));

                    if (infoHeader.RedMask != 0x00FF0000 || infoHeader.GreenMask != 0x0000FF00 || infoHeader.BlueMask != 0x000000FF || infoHeader.AlphaMask != 0xFF000000)
                    {
                        throw std::runtime_error("Unsupported BMP masks for 32-bit BMP file compressed with BI_BITFIELDS");
                    }
                }
                else
                {
                    throw std::runtime_error("32-bit compressed BMP files are not supported");
                }
            }

            uint32_t channels = infoHeader.BitsPerPixel / 8;

            uint32_t rowSize = infoHeader.BitsPerPixel * infoHeader.Width + 31;
            rowSize /= 32;
            rowSize *= 4;

            Bitmap bitmap(infoHeader.Width, infoHeader.Height);

            file.seekg(header.Offset, std::ios::beg);

            std::vector<uint8_t> row(rowSize);

            for (int y = 0; y < infoHeader.Height; ++y)
            {
                file.read(reinterpret_cast<char*>(row.data()), rowSize);

                for (int x = 0; x < infoHeader.Width; ++x)
                {
                    uint8_t b = row[x * channels];
                    uint8_t g = row[x * channels + 1];
                    uint8_t r = row[x * channels + 2];
                    uint8_t a = 255;

                    if (infoHeader.BitsPerPixel == 32)
                    {
                        a = row[x * channels + 3];
                    }

                    bitmap.SetPixel(x, infoHeader.Height - y - 1, ColorRGBA(r, g, b, a));
                }
            }

            return bitmap;
        }

        static void Save(const std::string& path, const Bitmap& bitmap, bool withAlpha = false)
        {
            Header header;
            InfoHeader infoHeader;
            
            uint16_t bitsPerPixel = withAlpha ? 32 : 24;
            int bytesPerPixel = bitsPerPixel / 8;
            int rowSize = ((bitmap.GetWidth() * bytesPerPixel + 3) / 4) * 4;

            header.Type = 0x4D42;
            header.Size = 14 + (withAlpha ? 56 : 40) + rowSize * bitmap.GetHeight();
            header.Reserved1 = 0;
            header.Reserved2 = 0;
            header.Offset = 14 + (withAlpha ? 56 : 40);
            
            infoHeader.Size = withAlpha ? 56 : 40;
            infoHeader.Width = bitmap.GetWidth();
            infoHeader.Height = bitmap.GetHeight();
            infoHeader.Planes = 1;
            infoHeader.BitsPerPixel = bitsPerPixel;
            infoHeader.Compression = withAlpha ? 3 : 0; // 3 is BI_BITFIELDS for alpha
            infoHeader.ImageSize = rowSize * bitmap.GetHeight();
            infoHeader.XPixelsPerMeter = 0;
            infoHeader.YPixelsPerMeter = 0;
            infoHeader.ColorUsed = 0;
            infoHeader.ColorImportant = 0;
            
            if (withAlpha) {
                infoHeader.RedMask = 0x00FF0000;
                infoHeader.GreenMask = 0x0000FF00;
                infoHeader.BlueMask = 0x000000FF;
                infoHeader.AlphaMask = 0xFF000000;
            }

            std::ofstream file(path, std::ios::binary);

            if (!file)
            {
                throw std::runtime_error("Unable to open file for writing: " + path);
            }

            file.write(reinterpret_cast<const char*>(&header.Type), sizeof(header.Type));
            file.write(reinterpret_cast<const char*>(&header.Size), sizeof(header.Size));
            file.write(reinterpret_cast<const char*>(&header.Reserved1), sizeof(header.Reserved1));
            file.write(reinterpret_cast<const char*>(&header.Reserved2), sizeof(header.Reserved2));
            file.write(reinterpret_cast<const char*>(&header.Offset), sizeof(header.Offset));

            file.write(reinterpret_cast<const char*>(&infoHeader.Size), sizeof(infoHeader.Size));
            file.write(reinterpret_cast<const char*>(&infoHeader.Width), sizeof(infoHeader.Width));
            file.write(reinterpret_cast<const char*>(&infoHeader.Height), sizeof(infoHeader.Height));
            file.write(reinterpret_cast<const char*>(&infoHeader.Planes), sizeof(infoHeader.Planes));
            file.write(reinterpret_cast<const char*>(&infoHeader.BitsPerPixel), sizeof(infoHeader.BitsPerPixel));
            file.write(reinterpret_cast<const char*>(&infoHeader.Compression), sizeof(infoHeader.Compression));
            file.write(reinterpret_cast<const char*>(&infoHeader.ImageSize), sizeof(infoHeader.ImageSize));
            file.write(reinterpret_cast<const char*>(&infoHeader.XPixelsPerMeter), sizeof(infoHeader.XPixelsPerMeter));
            file.write(reinterpret_cast<const char*>(&infoHeader.YPixelsPerMeter), sizeof(infoHeader.YPixelsPerMeter));
            file.write(reinterpret_cast<const char*>(&infoHeader.ColorUsed), sizeof(infoHeader.ColorUsed));
            file.write(reinterpret_cast<const char*>(&infoHeader.ColorImportant), sizeof(infoHeader.ColorImportant));
            
            if (withAlpha) {
                file.write(reinterpret_cast<const char*>(&infoHeader.RedMask), sizeof(infoHeader.RedMask));
                file.write(reinterpret_cast<const char*>(&infoHeader.GreenMask), sizeof(infoHeader.GreenMask));
                file.write(reinterpret_cast<const char*>(&infoHeader.BlueMask), sizeof(infoHeader.BlueMask));
                file.write(reinterpret_cast<const char*>(&infoHeader.AlphaMask), sizeof(infoHeader.AlphaMask));
            }

            for (int y = bitmap.GetHeight() - 1; y >= 0; y--)
            {
                for (int x = 0; x < bitmap.GetWidth(); x++)
                {
                    ColorRGBA color = bitmap.GetPixel(x, y);
                    uint8_t b = static_cast<uint8_t>(color.B * 255);
                    uint8_t g = static_cast<uint8_t>(color.G * 255);
                    uint8_t r = static_cast<uint8_t>(color.R * 255);

                    if (!withAlpha) {
                        // Premultiply alpha if saving without alpha channel
                        b = static_cast<uint8_t>(color.B * 255 * color.A);
                        g = static_cast<uint8_t>(color.G * 255 * color.A);
                        r = static_cast<uint8_t>(color.R * 255 * color.A);
                    }

                    file.write(reinterpret_cast<const char*>(&b), sizeof(b));
                    file.write(reinterpret_cast<const char*>(&g), sizeof(g));
                    file.write(reinterpret_cast<const char*>(&r), sizeof(r));
                    
                    if (withAlpha) {
                        uint8_t a = static_cast<uint8_t>(color.A * 255);
                        file.write(reinterpret_cast<const char*>(&a), sizeof(a));
                    }
                }

                uint32_t padding = 0;
                int paddingSize = rowSize - bitmap.GetWidth() * bytesPerPixel;\
                
                if (paddingSize > 0) {
                    file.write(reinterpret_cast<const char*>(&padding), paddingSize);
                }
            }

            file.close();
        }

    private:
        struct Header
        {
            uint16_t Type;
            uint32_t Size;
            uint16_t Reserved1;
            uint16_t Reserved2;
            uint32_t Offset;
        };

        struct InfoHeader
        {
            uint32_t Size;
            int32_t Width;
            int32_t Height;
            uint16_t Planes;
            uint16_t BitsPerPixel;
            uint32_t Compression;
            uint32_t ImageSize;
            int32_t XPixelsPerMeter;
            int32_t YPixelsPerMeter;
            uint32_t ColorUsed;
            uint32_t ColorImportant;
            uint32_t RedMask;
            uint32_t GreenMask;
            uint32_t BlueMask;
            uint32_t AlphaMask;
        };
    };
}