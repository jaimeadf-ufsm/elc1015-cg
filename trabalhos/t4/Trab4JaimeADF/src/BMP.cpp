#include "BMP.h"

#include <fstream>
#include <cstdint>
#include <vector>
#include <cstring>

Image BMP::Load(const std::string& filename)
{
    Image image;

    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return image;
    }

    char fileHeader[14];
    file.read(fileHeader, 14);
      if (fileHeader[0] != 'B' || fileHeader[1] != 'M') {
        file.close();
        return image;
    }
    
    uint32_t dataOffset;
    memcpy(&dataOffset, &fileHeader[10], sizeof(uint32_t));
      char dibHeader[40];
    file.read(dibHeader, 40);
    
    int32_t width, height;
    uint16_t bitsPerPixel;
    uint32_t compression;
    
    memcpy(&width, &dibHeader[4], sizeof(int32_t));
    memcpy(&height, &dibHeader[8], sizeof(int32_t));
    memcpy(&bitsPerPixel, &dibHeader[14], sizeof(uint16_t));
    memcpy(&compression, &dibHeader[16], sizeof(uint32_t));
    
    if (bitsPerPixel != 24 || compression != 0) {
        file.close();
        return image;
    }
    
    bool topDown = height < 0;
    if (topDown) {
        height = -height;
    }
    
    image.Resize(width, height);
    
    int bytesPerPixel = 3;
    int rowSize = width * bytesPerPixel;
    int padding = (4 - (rowSize % 4)) % 4;
    int paddedRowSize = rowSize + padding;
    
    file.seekg(dataOffset);
    
    std::vector<uint8_t> rowData(paddedRowSize);    for (int y = 0; y < height; y++) {
        file.read(reinterpret_cast<char*>(rowData.data()), paddedRowSize);
        
        // BMP files are stored bottom-up by default, but we need to account for
        // the coordinate system used when displaying. Since OpenGL uses bottom-left origin
        // and our Image class uses top-left origin, we need to flip for bottom-up BMPs
        int actualY = topDown ? (height - 1 - y) : y;
        
        for (int x = 0; x < width; x++) {
            int pixelIndex = x * bytesPerPixel;
            
            uint8_t blue = rowData[pixelIndex];
            uint8_t green = rowData[pixelIndex + 1];
            uint8_t red = rowData[pixelIndex + 2];
            
            ColorRGB color(red / 255.0f, green / 255.0f, blue / 255.0f);
            image.SetPixel(x, actualY, color);
        }
    }
    
    file.close();

    return image;
}
