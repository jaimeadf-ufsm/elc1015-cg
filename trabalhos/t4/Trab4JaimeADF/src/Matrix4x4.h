/**
 * @file Matrix4x4.h
 * @brief 4x4 matrix class for 3D transformations and linear algebra operations
 */

#pragma once

#include <array>
#include "Vector3.h"
#include "Vector4.h"

/**
 * @struct Matrix4x4
 * @brief 4x4 matrix structure for 3D transformations and mathematical operations
 */
struct Matrix4x4
{
    std::array<std::array<float, 4>, 4> Elements;

    Matrix4x4();
    Matrix4x4(float diagonal);
    Matrix4x4(const std::array<std::array<float, 4>, 4>& elements);

    static Matrix4x4 Identity();
    static Matrix4x4 Zero();

    static Matrix4x4 Translate(const Vector3& translation);
    static Matrix4x4 Scale(const Vector3& scale);
    static Matrix4x4 Rotate(const Vector3& axis, float angle);

    static Matrix4x4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
    static Matrix4x4 Ortographic(float left, float right, float bottom, float top, float near, float far);
    static Matrix4x4 Perspective(float fovY, float aspectRatio, float near, float far);

    Matrix4x4 operator+(const Matrix4x4& other) const;
    Matrix4x4 operator-(const Matrix4x4& other) const;
    Matrix4x4 operator*(const Matrix4x4& other) const;
    Matrix4x4 operator*(float scalar) const;

    Matrix4x4& operator+=(const Matrix4x4& other);
    Matrix4x4& operator-=(const Matrix4x4& other);
    Matrix4x4& operator*=(const Matrix4x4& other);
    Matrix4x4& operator*=(float scalar);

    Vector4 operator*(const Vector4& vector) const;

    std::array<float, 4>& operator[](int row);
    const std::array<float, 4>& operator[](int row) const;
};

Matrix4x4 operator*(float scalar, const Matrix4x4& matrix);
