#include <cmath>

#include "Vector2.h"

Vector2::Vector2() : X(0), Y(0) {}
Vector2::Vector2(float x, float y) : X(x), Y(y) {}

float Vector2::ProjectOnto(const Vector2& other) const
{
    float dotProduct = Dot(other);
    float otherMagnitudeSquared = other.MagnitudeSquared();

    if (otherMagnitudeSquared == 0.0f)
    {
        return 0.0f;
    }

    return dotProduct / otherMagnitudeSquared;
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
    return std::sqrt(X * X + Y * Y);
}

float Vector2::MagnitudeSquared() const
{
    return X * X + Y * Y;
}

float Vector2::Angle() const
{
    return std::atan2f(Y, X);
}

Vector2 Vector2::Normalized() const
{
    float magnitude = Magnitude();

    if (magnitude == 0.0f)
    {
        return Vector2(0, 0); // Avoid division by zero
    }

    return Vector2(X / magnitude, Y / magnitude);
}

Vector2 Vector2::operator-() const
{
    return Vector2(-X, -Y);
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

bool Vector2::operator==(const Vector2& other) const
{
    return (X == other.X && Y == other.Y);
}

bool Vector2::operator!=(const Vector2& other) const
{
    return !(*this == other);
}

Vector2 operator*(float scalar, const Vector2& vector)
{
    return Vector2(vector.X * scalar, vector.Y * scalar);
}

Vector2 operator/(float scalar, const Vector2& vector)
{
    return Vector2(scalar / vector.X, scalar / vector.Y);
}

std::ostream& operator<<(std::ostream& os, const Vector2& vector)
{
    os << '(' << vector.X << ", " << vector.Y << ')';
    return os;
}