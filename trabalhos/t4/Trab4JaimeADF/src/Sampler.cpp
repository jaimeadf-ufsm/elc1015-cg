
#include "Sampler.h"

Sampler::Sampler() : Sampler(SamplingSource::Color, ColorRGB(1.0f, 1.0f, 1.0f), nullptr)
{
}

Sampler::Sampler(SamplingSource source, const ColorRGB& color, const std::shared_ptr<Image>& image) :
    m_Source(source),
    m_Color(color),
    m_Image(image)
{
}

ColorRGB Sampler::Sample(const Vector2& uv) const
{
    switch (m_Source)
    {
        case SamplingSource::Coordinates:
            return ColorRGB(uv.X, uv.Y, 1.0f - (uv.X + uv.Y) / 2.0f);
        case SamplingSource::Color:
            return m_Color;
        case SamplingSource::Image:
            {
                std::size_t width = m_Image->GetWidth();
                std::size_t height = m_Image->GetHeight();

                if (width == 0 || height == 0)
                {
                    return ColorRGB();
                }

                std::size_t x = static_cast<std::size_t>(uv.X * width) % width;
                std::size_t y = static_cast<std::size_t>(uv.Y * height) % height;

                return m_Image->GetPixel(x, y);
            }
        default:
            return ColorRGB();
    }
}

Sampler Sampler::CreateCoordinatesSampler()
{
    return Sampler(SamplingSource::Coordinates, ColorRGB(), nullptr);
}

Sampler Sampler::CreateColorSampler(const ColorRGB& color)
{
    return Sampler(SamplingSource::Color, color, nullptr);
}

Sampler Sampler::CreateImageSampler(const Image& image)
{
    return Sampler(SamplingSource::Image, ColorRGB(), std::make_shared<Image>(image));
}