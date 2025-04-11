#pragma once

#include "Math.h"

namespace yap
{
    class ColorRGB;
    class ColorRGBA;
    class ColorHSV;
    class ColorHSVA;

    class ColorRGB
    {
    public:
        float R, G, B;

        ColorRGB() : R(0), G(0), B(0) {}
        ColorRGB(float r, float g, float b) : R(r), G(g), B(b) {}
        ColorRGB(int r, int g, int b) : R(r / 255.0f), G(g / 255.0f), B(b / 255.0f) {}

        ColorHSV ToHSV() const;

        bool operator==(const ColorRGB& other) const
        {
            return R == other.R && G == other.G && B == other.B;
        }

        bool operator!=(const ColorRGB& other) const
        {
            return !(*this == other);
        }

        ColorRGB& operator+=(const ColorRGB& other)
        {
            R += other.R;
            G += other.G;
            B += other.B;
            return *this;
        }

        ColorRGB& operator-=(const ColorRGB& other)
        {
            R -= other.R;
            G -= other.G;
            B -= other.B;
            return *this;
        }

        ColorRGB& operator*=(const ColorRGB& other)
        {
            R *= other.R;
            G *= other.G;
            B *= other.B;
            return *this;
        }

        ColorRGB& operator/=(const ColorRGB& other)
        {
            R /= other.R;
            G /= other.G;
            B /= other.B;
            return *this;
        }

        ColorRGB& operator+=(float scalar)
        {
            R += scalar;
            G += scalar;
            B += scalar;
            return *this;
        }

        ColorRGB& operator-=(float scalar)
        {
            R -= scalar;
            G -= scalar;
            B -= scalar;
            return *this;
        }

        ColorRGB& operator*=(float scalar)
        {
            R *= scalar;
            G *= scalar;
            B *= scalar;
            return *this;
        }

        ColorRGB& operator/=(float scalar)
        {
            R /= scalar;
            G /= scalar;
            B /= scalar;
            return *this;
        }

        ColorRGB operator+(const ColorRGB& other) const
        {
            return ColorRGB(R + other.R, G + other.G, B + other.B);
        }

        ColorRGB operator-(const ColorRGB& other) const
        {
            return ColorRGB(R - other.R, G - other.G, B - other.B);
        }

        ColorRGB operator*(const ColorRGB& other) const
        {
            return ColorRGB(R * other.R, G * other.G, B * other.B);
        }

        ColorRGB operator/(const ColorRGB& other) const
        {
            return ColorRGB(R / other.R, G / other.G, B / other.B);
        }

        ColorRGB operator+(float scalar) const
        {
            return ColorRGB(R + scalar, G + scalar, B + scalar);
        }

        ColorRGB operator-(float scalar) const
        {
            return ColorRGB(R - scalar, G - scalar, B - scalar);
        }

        ColorRGB operator*(float scalar) const
        {
            return ColorRGB(R * scalar, G * scalar, B * scalar);
        }

        ColorRGB operator/(float scalar) const
        {
            return ColorRGB(R / scalar, G / scalar, B / scalar);
        }

        static ColorRGB Lerp(const ColorRGB& c1, const ColorRGB& c2, float t)
        {
            return ColorRGB(
                yap::Lerp(c1.R, c2.R, t),
                yap::Lerp(c1.G, c2.G, t),
                yap::Lerp(c1.B, c2.B, t)
            );
        }
    };

    struct ColorHSV
    {
        float H, S, V;

        ColorHSV() : H(0), S(0), V(0) {}
        ColorHSV(float h, float s, float v) : H(h), S(s), V(v) {}

        ColorRGB ToRGB() const;

        bool operator==(const ColorHSV& other) const
        {
            return H == other.H && S == other.S && V == other.V;
        }

        bool operator!=(const ColorHSV& other) const
        {
            return !(*this == other);
        }
    };

    class ColorRGBA : public ColorRGB
    {
    public:
        float A;

        ColorRGBA() : ColorRGB(), A(1.0f) {}
        ColorRGBA(float r, float g, float b, float a = 1.0f) : ColorRGB(r, g, b), A(a) {}
        ColorRGBA(int r, int g, int b, int a = 255) : ColorRGB(r, g, b), A(a / 255.0f) {}
        ColorRGBA(const ColorRGB& rgb, float a = 1.0f) : ColorRGB(rgb), A(a) {}

        ColorRGB CompositeOver(const ColorRGB& background) const
        {
            return ColorRGB(
                R * A + background.R * (1.0f - A),
                G * A + background.G * (1.0f - A),
                B * A + background.B * (1.0f - A)
            );
        }

        ColorRGBA CompositeOver(const ColorRGBA& background) const
        {
            return ColorRGBA(
                R * A + background.R * (1.0f - A),
                G * A + background.G * (1.0f - A),
                B * A + background.B * (1.0f - A),
                A + background.A * (1.0f - A)
            );
        }

        ColorHSVA ToHSVA() const;

        ColorRGBA& operator+=(const ColorRGBA& other)
        {
            R += other.R;
            G += other.G;
            B += other.B;
            A += other.A;
            return *this;
        }

        ColorRGBA& operator-=(const ColorRGBA& other)
        {
            R -= other.R;
            G -= other.G;
            B -= other.B;
            A -= other.A;
            return *this;
        }

        ColorRGBA& operator*=(const ColorRGBA& other)
        {
            R *= other.R;
            G *= other.G;
            B *= other.B;
            A *= other.A;
            return *this;
        }

        ColorRGBA& operator/=(const ColorRGBA& other)
        {
            R /= other.R;
            G /= other.G;
            B /= other.B;
            A /= other.A;
            return *this;
        }

        ColorRGBA& operator+=(float scalar)
        {
            R += scalar;
            G += scalar;
            B += scalar;
            A += scalar;
            return *this;
        }

        ColorRGBA& operator-=(float scalar)
        {
            R -= scalar;
            G -= scalar;
            B -= scalar;
            A -= scalar;
            return *this;
        }

        ColorRGBA& operator*=(float scalar)
        {
            R *= scalar;
            G *= scalar;
            B *= scalar;
            A *= scalar;
            return *this;
        }

        ColorRGBA& operator/=(float scalar)
        {
            R /= scalar;
            G /= scalar;
            B /= scalar;
            A /= scalar;
            return *this;
        }

        ColorRGBA operator+(const ColorRGBA& other) const
        {
            return ColorRGBA(R + other.R, G + other.G, B + other.B, A + other.A);
        }

        ColorRGBA operator-(const ColorRGBA& other) const
        {
            return ColorRGBA(R - other.R, G - other.G, B - other.B, A - other.A);
        }

        ColorRGBA operator*(const ColorRGBA& other) const
        {
            return ColorRGBA(R * other.R, G * other.G, B * other.B, A * other.A);
        }

        ColorRGBA operator/(const ColorRGBA& other) const
        {
            return ColorRGBA(R / other.R, G / other.G, B / other.B, A / other.A);
        }

        ColorRGBA operator+(float scalar) const
        {
            return ColorRGBA(R + scalar, G + scalar, B + scalar, A + scalar);
        }

        ColorRGBA operator-(float scalar) const
        {
            return ColorRGBA(R - scalar, G - scalar, B - scalar, A - scalar);
        }

        ColorRGBA operator*(float scalar) const
        {
            return ColorRGBA(R * scalar, G * scalar, B * scalar, A * scalar);
        }

        ColorRGBA operator/(float scalar) const
        {
            return ColorRGBA(R / scalar, G / scalar, B / scalar, A / scalar);
        }

        bool operator==(const ColorRGBA& other) const
        {
            return R == other.R && G == other.G && B == other.B && A == other.A;
        }

        bool operator!=(const ColorRGBA& other) const
        {
            return !(*this == other);
        }

        static ColorRGBA Lerp(const ColorRGBA& c1, const ColorRGBA& c2, float t)
        {
            return ColorRGBA(
                c1.R * (1 - t) + c2.R * t,
                c1.G * (1 - t) + c2.G * t,
                c1.B * (1 - t) + c2.B * t,
                c1.A * (1 - t) + c2.A * t
            );
        }

        static ColorRGBA Clamp(const ColorRGBA& color, float min = 0.0f, float max = 1.0f)
        {
            return ColorRGBA(
                yap::Clamp(color.R, min, max),
                yap::Clamp(color.G, min, max),
                yap::Clamp(color.B, min, max),
                yap::Clamp(color.A, min, max)
            );
        }
    };

    class ColorHSVA : public ColorHSV
    {
    public:
        float A;

        ColorHSVA() : ColorHSV(), A(1.0f) {}
        ColorHSVA(float h, float s, float v, float a) : ColorHSV(h, s, v), A(a) {}
        ColorHSVA(const ColorHSV& hsv, float a = 1.0f) : ColorHSV(hsv), A(a) {}

        ColorRGBA ToRGBA() const;

        bool operator==(const ColorHSVA& other) const
        {
            return H == other.H && S == other.S && V == other.V && A == other.A;
        }

        bool operator!=(const ColorHSVA& other) const
        {
            return !(*this == other);
        }
    };

    ColorHSV ColorRGB::ToHSV() const
    {
        ColorHSV result;

        float max = std::max(R, std::max(G, B));
        float min = std::min(R, std::min(G, B));

        float delta = max - min;

        result.V = max;

        if (delta < 0.00001f)
        {
            result.S = 0;
            result.H = 0;

            return result;
        }

        if (max > 0.0f)
        {
            result.S = delta / max;
        }
        else
        {
            result.S = 0.0f;
            result.H = 0.0f;

            return result;
        }

        if (R >= max)
        {
            result.H = (G - B) / delta;
        }
        else if (G >= max)
        {
            result.H = 2.0f + (B - R) / delta;
        }
        else
        {
            result.H = 4.0f + (R - G) / delta;
        }

        result.H *= 60.0f;

        if (result.H < 0.0f)
        {
            result.H += 360.0f;
        }

        return result;
    }

    ColorRGB ColorHSV::ToRGB() const
    {
        if (S <= 0.0f)
        {
            return ColorRGB(V, V, V);
        }

        float h = H / 60.0f;
        int i = h;
        float f = h - i;
        float p = V * (1.0f - S);
        float q = V * (1.0f - f * S);
        float t = V * (1.0f - (1.0f - f) * S);

        switch (i % 6)
        {
        case 0:
            return ColorRGB(V, t, p);
        case 1:
            return ColorRGB(q, V, p);
        case 2:
            return ColorRGB(p, V, t);
        case 3:
            return ColorRGB(p, q, V);
        case 4:
            return ColorRGB(t, p, V);
        case 5:
            return ColorRGB(V, p, q);
        }

        return ColorRGB(0, 0, 0);
    }

    ColorHSVA ColorRGBA::ToHSVA() const
    {
        return ColorHSVA(ColorRGB(R, G, B).ToHSV(), A);
    }

    ColorRGBA ColorHSVA::ToRGBA() const
    {
        return ColorRGBA(ColorHSV(H, S, V).ToRGB(), A);
    }
}