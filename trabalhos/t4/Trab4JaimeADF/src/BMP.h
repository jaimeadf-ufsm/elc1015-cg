#pragma once

#include <string>
#include "Image.h"

class BMP
{
public:
    static Image Load(const std::string& filename);
};