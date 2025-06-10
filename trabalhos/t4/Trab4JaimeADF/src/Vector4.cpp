#include <cmath>

#include "Vector4.h"
#include "Color.h"

Vector4::Vector4() : Vector4(0.0f, 0.0f, 0.0f, 0.0f)
{

}

Vector4::Vector4(const Vector2& xy, float z, float w) : X(xy.X), Y(xy.Y), Z(z), W(w)
{

}

Vector4::Vector4(const Vector3& xyz, float w) : X(xyz.X), Y(xyz.Y), Z(xyz.Z), W(w)
{

}

Vector4::Vector4(const ColorRGBA& color) : Vector4(color.R, color.G, color.B, color.A)
{

}

Vector4::Vector4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w)
{

}

Vector4 Vector4::Normalize() const
{
    float magnitude = Magnitude();

    if (magnitude == 0.0f)
    {
        return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    return Vector4(X / magnitude, Y / magnitude, Z / magnitude, W / magnitude);
}

float Vector4::Dot(const Vector4& other) const
{
    return X * other.X + Y * other.Y + Z * other.Z + W * other.W;
}

float Vector4::Magnitude() const
{
    return std::sqrt(MagnitudeSquared());
}

float Vector4::MagnitudeSquared() const
{
    return X * X + Y * Y + Z * Z + W * W;
}

Vector3 Vector4::ToCartesianPosition() const
{
    return Vector3(X / W, Y / W, Z / W);
}

Vector3 Vector4::ToCartesianDirection() const
{
    return Vector3(X, Y, Z);
}

Vector4 Vector4::operator+(const Vector4& other) const
{
    return Vector4(X + other.X, Y + other.Y, Z + other.Z, W + other.W);
}

Vector4 Vector4::operator-(const Vector4& other) const
{
    return Vector4(X - other.X, Y - other.Y, Z - other.Z, W - other.W);
}

Vector4 Vector4::operator*(const Vector4& other) const
{
    return Vector4(X * other.X, Y * other.Y, Z * other.Z, W * other.W);
}

Vector4 Vector4::operator/(const Vector4& other) const
{
    return Vector4(X / other.X, Y / other.Y, Z / other.Z, W / other.W);
}

Vector4 Vector4::operator*(float scalar) const
{
    return Vector4(X * scalar, Y * scalar, Z * scalar, W * scalar);
}

Vector4 Vector4::operator/(float scalar) const
{
    return Vector4(X / scalar, Y / scalar, Z / scalar, W / scalar);
}

Vector4& Vector4::operator+=(const Vector4& other)
{
    X += other.X;
    Y += other.Y;
    Z += other.Z;
    W += other.W;
    return *this;
}

Vector4& Vector4::operator-=(const Vector4& other)
{
    X -= other.X;
    Y -= other.Y;
    Z -= other.Z;
    W -= other.W;
    return *this;
}

Vector4& Vector4::operator*=(const Vector4& other)
{
    X *= other.X;
    Y *= other.Y;
    Z *= other.Z;
    W *= other.W;
    return *this;
}

Vector4& Vector4::operator/=(const Vector4& other)
{
    X /= other.X;
    Y /= other.Y;
    Z /= other.Z;
    W /= other.W;
    return *this;
}

Vector4& Vector4::operator*=(float scalar)
{
    X *= scalar;
    Y *= scalar;
    Z *= scalar;
    W *= scalar;
    return *this;
}

Vector4& Vector4::operator/=(float scalar)
{
    X /= scalar;
    Y /= scalar;
    Z /= scalar;
    W /= scalar;
    return *this;
}

bool Vector4::operator==(const Vector4& other) const
{
    return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
}