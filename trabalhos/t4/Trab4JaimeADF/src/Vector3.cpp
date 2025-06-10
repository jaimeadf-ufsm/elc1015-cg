#include <cmath>

#include "Vector3.h"
#include "Color.h"

Vector3::Vector3() : Vector3(0.0f, 0.0f, 0.0f)
{

}

Vector3::Vector3(const Vector2& xy, float z) : X(xy.X), Y(xy.Y), Z(z)
{

}

Vector3::Vector3(const ColorRGB& color) : Vector3(color.R, color.G, color.B)
{

}

Vector3::Vector3(float x, float y, float z) : X(x), Y(y), Z(z)
{

}

Vector3 Vector3::Normalize() const
{
    float magnitude = Magnitude();

    if (magnitude < 1e-6f)
    {
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    return Vector3(X / magnitude, Y / magnitude, Z / magnitude);
}

float Vector3::Dot(const Vector3& other) const
{
    return X * other.X + Y * other.Y + Z * other.Z;
}

Vector3 Vector3::Cross(const Vector3& other) const
{
    return Vector3(
        Y * other.Z - Z * other.Y,
        Z * other.X - X * other.Z,
        X * other.Y - Y * other.X
    );
}

float Vector3::Magnitude() const
{
    return std::sqrt(MagnitudeSquared());
}

float Vector3::MagnitudeSquared() const
{
    return X * X + Y * Y + Z * Z;
}

Vector3 Vector3::operator+(const Vector3& other) const
{
    return Vector3(X + other.X, Y + other.Y, Z + other.Z);
}

Vector3 Vector3::operator-(const Vector3& other) const
{
    return Vector3(X - other.X, Y - other.Y, Z - other.Z);
}

Vector3 Vector3::operator*(const Vector3& other) const
{
    return Vector3(X * other.X, Y * other.Y, Z * other.Z);
}

Vector3 Vector3::operator/(const Vector3& other) const
{
    return Vector3(X / other.X, Y / other.Y, Z / other.Z);
}

Vector3 Vector3::operator*(float scalar) const
{
    return Vector3(X * scalar, Y * scalar, Z * scalar);
}

Vector3 Vector3::operator/(float scalar) const
{
    return Vector3(X / scalar, Y / scalar, Z / scalar);
}

Vector3& Vector3::operator+=(const Vector3& other)
{
    X += other.X;
    Y += other.Y;
    Z += other.Z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
    X -= other.X;
    Y -= other.Y;
    Z -= other.Z;
    return *this;
}

Vector3& Vector3::operator*=(const Vector3& other)
{
    X *= other.X;
    Y *= other.Y;
    Z *= other.Z;
    return *this;
}

Vector3& Vector3::operator/=(const Vector3& other)
{
    X /= other.X;
    Y /= other.Y;
    Z /= other.Z;
    return *this;
}

Vector3& Vector3::operator*=(float scalar)
{
    X *= scalar;
    Y *= scalar;
    Z *= scalar;
    return *this;
}

Vector3& Vector3::operator/=(float scalar)
{
    X /= scalar;
    Y /= scalar;
    Z /= scalar;
    return *this;
}

bool Vector3::operator==(const Vector3& other) const
{
    return X == other.X && Y == other.Y && Z == other.Z;
}