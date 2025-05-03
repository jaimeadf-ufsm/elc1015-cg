#include "Vector2.h"

Vector2::Vector2() : X(0), Y(0) {}
Vector2::Vector2(float x, float y) : X(x), Y(y) {}

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



