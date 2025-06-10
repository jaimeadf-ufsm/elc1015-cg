/**
 * @file BMP.h
 * @brief BMP image file format loader
 */

#pragma once

#include <string>
#include "Image.h"

/**
 * @class BMP
 * @brief Provides functionality to load BMP image files
 */
class BMP
{
public:
    static Image Load(const std::string& filename);
};