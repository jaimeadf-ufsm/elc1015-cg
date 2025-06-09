#include <cmath>

#include "Vector2.h"

Vector2::Vector2() : Vector2(0.0f, 0.0f)
{

}

Vector2::Vector2(float x, float y) : X(x), Y(y)
{

}

Vector2 Vector2::Normalize() const
{
    float magnitude = Magnitude();

    if (magnitude == 0.0f)
    {
        return Vector2(0.0f, 0.0f);
    }

    return Vector2(X / magnitude, Y / magnitude);
}

float Vector2::Dot(const Vector2& other) const
{
    return X * other.X + Y * other.Y;
}

float Vector2::Cross(const Vector2& other) const
{
    return X * other.Y - Y * other.X;
}

float Vector2::Magnitude() const
{
    return std::sqrt(MagnitudeSquared());
}

float Vector2::MagnitudeSquared() const
{
    return X * X + Y * Y;
}

Vector2 Vector2::Floor() const
{
    return Vector2(std::floor(X), std::floor(Y));
}

Vector2 Vector2::Ceil() const
{
    return Vector2(std::ceil(X), std::ceil(Y));
}

Vector2 Vector2::Round() const
{
    return Vector2(std::round(X), std::round(Y));
}

Vector2 Vector2::operator+(const Vector2& other) const
{
    return Vector2(X + other.X, Y + other.Y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
    return Vector2(X - other.X, Y - other.Y);
}

Vector2 Vector2::operator*(const Vector2& other) const
{
    return Vector2(X * other.X, Y * other.Y);
}

Vector2 Vector2::operator/(const Vector2& other) const
{
    return Vector2(X / other.X, Y / other.Y);
}

Vector2 Vector2::operator*(float scalar) const
{
    return Vector2(X * scalar, Y * scalar);
}

Vector2 Vector2::operator/(float scalar) const
{
    return Vector2(X / scalar, Y / scalar);
}

Vector2& Vector2::operator+=(const Vector2& other)
{
    X += other.X;
    Y += other.Y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& other)
{
    X -= other.X;
    Y -= other.Y;
    return *this;
}

Vector2& Vector2::operator*=(const Vector2& other)
{
    X *= other.X;
    Y *= other.Y;
    return *this;
}

Vector2& Vector2::operator/=(const Vector2& other)
{
    X /= other.X;
    Y /= other.Y;
    return *this;
}

Vector2& Vector2::operator*=(float scalar)
{
    X *= scalar;
    Y *= scalar;
    return *this;
}

Vector2& Vector2::operator/=(float scalar)
{
    X /= scalar;
    Y /= scalar;
    return *this;
}
