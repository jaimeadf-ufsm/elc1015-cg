#include <cmath>
#include <cstring>
#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
    memset(Elements.data(), 0, sizeof(Elements));
}

Matrix4x4::Matrix4x4(float diagonal)
{
    memset(Elements.data(), 0, sizeof(Elements));
    Elements[0][0] = diagonal;
    Elements[1][1] = diagonal;
    Elements[2][2] = diagonal;
    Elements[3][3] = diagonal;
}

Matrix4x4::Matrix4x4(const std::array<std::array<float, 4>, 4>& elements) : Elements(elements)
{
}

Matrix4x4 Matrix4x4::Identity()
{
    return Matrix4x4(1.0f);
}

Matrix4x4 Matrix4x4::Zero()
{
    return Matrix4x4(0.0f);
}

Matrix4x4 Matrix4x4::Translate(const Vector3& translation)
{
    Matrix4x4 result = Identity();
    result[0][3] = translation.X;
    result[1][3] = translation.Y;
    result[2][3] = translation.Z;

    return result;
}

Matrix4x4 Matrix4x4::Scale(const Vector3& scale)
{
    Matrix4x4 result = Identity();
    result[0][0] = scale.X;
    result[1][1] = scale.Y;
    result[2][2] = scale.Z;

    return result;
}

Matrix4x4 Matrix4x4::Rotate(const Vector3& axis, float angle)
{
    Matrix4x4 result = Identity();
    
    float c = std::cos(angle);
    float s = std::sin(angle);
    float t = 1.0f - c;

    result[0][0] = t * axis.X * axis.X + c;
    result[0][1] = t * axis.X * axis.Y - s * axis.Z;
    result[0][2] = t * axis.X * axis.Z + s * axis.Y;

    result[1][0] = t * axis.X * axis.Y + s * axis.Z;
    result[1][1] = t * axis.Y * axis.Y + c;
    result[1][2] = t * axis.Y * axis.Z - s * axis.X;

    result[2][0] = t * axis.X * axis.Z - s * axis.Y;
    result[2][1] = t * axis.Y * axis.Z + s * axis.X;
    result[2][2] = t * axis.Z * axis.Z + c;

    return result;
}

Matrix4x4 Matrix4x4::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
    Matrix4x4 result = Identity();

    Vector3 f = (center - eye).Normalize();     // Forward
    Vector3 s = f.Cross(up).Normalize();        // Right
    Vector3 u = s.Cross(f);                     // Up (recomputed to be orthogonal)


    result[0][0] = s.X;
    result[0][1] = s.Y;
    result[0][2] = s.Z;
    result[0][3] = -s.Dot(eye);

    result[1][0] = u.X;
    result[1][1] = u.Y;
    result[1][2] = u.Z;
    result[1][3] = -u.Dot(eye);

    result[2][0] = -f.X;
    result[2][1] = -f.Y;
    result[2][2] = -f.Z;
    result[2][3] = f.Dot(eye);

    result[3][0] = 0.0f;
    result[3][1] = 0.0f;
    result[3][2] = 0.0f;
    result[3][3] = 1.0f;

    return result;
}

Matrix4x4 Matrix4x4::Ortographic(float left, float right, float bottom, float top, float near, float far)
{
    Matrix4x4 result = Zero();
    
    result[0][0] = 2.0f / (right - left);
    result[1][1] = 2.0f / (top - bottom);
    result[2][2] = -2.0f / (far - near);
    result[3][3] = 1.0f;

    result[0][3] = -(right + left) / (right - left);
    result[1][3] = -(top + bottom) / (top - bottom);
    result[2][3] = -(far + near) / (far - near);

    return result;
}

Matrix4x4 Matrix4x4::Perspective(float fovY, float aspectRatio, float near, float far)
{
    Matrix4x4 result = Zero();
    
    float f = 1.0f / std::tan(fovY * 0.5f);
    float nf = 1.0f / (near - far);
    
    result[0][0] = f / aspectRatio;
    result[1][1] = f;
    result[2][2] = (far + near) * nf;
    result[2][3] = (2.0f * far * near) * nf;
    result[3][2] = -1.0f;
    result[3][3] = 0.0f;

    return result;
}

std::array<float, 4>& Matrix4x4::operator[](int row)
{
    return Elements[row];
}

const std::array<float, 4>& Matrix4x4::operator[](int row) const
{
    return Elements[row];
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other) const
{
    Matrix4x4 result;
    
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result[i][j] = Elements[i][j] + other[i][j];
        }
    }
    
    return result;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& other) const
{
    Matrix4x4 result;
    
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result[i][j] = Elements[i][j] - other[i][j];
        }
    }
    
    return result;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
{
    Matrix4x4 result;
    
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result[i][j] = 0.0f;
            for (int k = 0; k < 4; ++k)
            {
                result[i][j] += Elements[i][k] * other[k][j];
            }
        }
    }
    
    return result;
}

Matrix4x4 Matrix4x4::operator*(float scalar) const
{
    Matrix4x4 result;
    
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result[i][j] = Elements[i][j] * scalar;
        }
    }
    
    return result;
}

Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& other)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            Elements[i][j] += other[i][j];
        }
    }
    
    return *this;
}

Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& other)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            Elements[i][j] -= other[i][j];
        }
    }
    
    return *this;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& other)
{
    *this = *this * other;
    return *this;
}

Matrix4x4& Matrix4x4::operator*=(float scalar)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            Elements[i][j] *= scalar;
        }
    }
    
    return *this;
}

Vector4 Matrix4x4::operator*(const Vector4& vector) const
{
    return Vector4(
        Elements[0][0] * vector.X + Elements[0][1] * vector.Y + Elements[0][2] * vector.Z + Elements[0][3] * vector.W,
        Elements[1][0] * vector.X + Elements[1][1] * vector.Y + Elements[1][2] * vector.Z + Elements[1][3] * vector.W,
        Elements[2][0] * vector.X + Elements[2][1] * vector.Y + Elements[2][2] * vector.Z + Elements[2][3] * vector.W,
        Elements[3][0] * vector.X + Elements[3][1] * vector.Y + Elements[3][2] * vector.Z + Elements[3][3] * vector.W
    );
}

Matrix4x4 operator*(float scalar, const Matrix4x4& matrix)
{
    return matrix * scalar;
}
