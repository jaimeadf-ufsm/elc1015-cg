#pragma once

#include <memory>

#include "Color.h"
#include "Vector2.h"
#include "Image.h"

enum class SamplingSource
{
    Coordinates,
    Color,
    Image
};

class Sampler
{
public:
    Sampler();

    ColorRGB Sample(const Vector2& uv) const;

    static Sampler CreateCoordinatesSampler();
    static Sampler CreateColorSampler(const ColorRGB& color);
    static Sampler CreateImageSampler(const Image& image);

private:
    SamplingSource m_Source;

    ColorRGB m_Color;
    std::shared_ptr<Image> m_Image;

    Sampler(SamplingSource source, const ColorRGB& color, const std::shared_ptr<Image>& image);
};