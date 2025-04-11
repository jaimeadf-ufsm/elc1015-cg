#pragma once

#include "Axis.h"

/**
 * @file Vec2.h
 * @brief Defines a 2D vector class with various mathematical operations and utilities.
 * 
 * This file contains the `Vec2` struct, which represents a 2D vector with `X` and `Y` components.
 * It provides a wide range of methods for vector manipulation, including rotation, normalization,
 * clamping, and arithmetic operations.
 */

namespace yap
{
    /**
     * @struct Vec2
     * @brief Represents a 2D vector with `X` and `Y` components.
     * 
     * The `Vec2` struct provides methods for common vector operations such as rotation,
     * normalization, dot product, and more. It also supports operator overloading for
     * arithmetic operations.
     */
    struct Vec2
    {
        float X, Y; ///< The X and Y components of the vector.

        /**
         * @brief Default constructor. Initializes the vector to (0, 0).
         */
        Vec2() : X(0), Y(0) {}

        /**
         * @brief Constructor that initializes both components to the same value.
         * @param value The value to assign to both X and Y.
         */
        Vec2(float value) : X(value), Y(value) {}

        /**
         * @brief Constructor that initializes the vector with specific X and Y values.
         * @param x The X component.
         * @param y The Y component.
         */
        Vec2(float x, float y) : X(x), Y(y) {}

        /**
         * @brief Copy constructor.
         * @param other The vector to copy from.
         */
        Vec2(const Vec2& other) : X(other.X), Y(other.Y) {}

        /**
         * @brief Rotates the vector by a given angle (in radians).
         * @param angle The angle to rotate by, in radians.
         */
        void Rotate(float angle)
        {
            float cosAngle = cos(angle);
            float sinAngle = sin(angle);

            float newX = X * cosAngle - Y * sinAngle;
            float newY = X * sinAngle + Y * cosAngle;

            X = newX;
            Y = newY;
        }

        /**
         * @brief Rotates the vector around a pivot point by a given angle (in radians).
         * @param angle The angle to rotate by, in radians.
         * @param pivot The pivot point for the rotation.
         */
        void Rotate(float angle, const Vec2& pivot)
        {
            float cosAngle = cos(angle);
            float sinAngle = sin(angle);

            float newX = (X - pivot.X) * cosAngle - (Y - pivot.Y) * sinAngle + pivot.X;
            float newY = (X - pivot.X) * sinAngle + (Y - pivot.Y) * cosAngle + pivot.Y;

            X = newX;
            Y = newY;
        }

        /**
         * @brief Normalizes the vector to have a length of 1.
         * If the vector has zero length, it remains unchanged.
         */
        void Normalize()
        {
            float length = Length();

            if (length == 0)
            {
                X = 0;
                Y = 0;
            }

            X /= length;
            Y /= length;
        }

        /**
         * @brief Calculates the length (magnitude) of the vector.
         * @return The length of the vector.
         */
        float Length() const
        {
            return sqrt(X * X + Y * Y);
        }

        /**
         * @brief Calculates the squared length of the vector.
         * @return The squared length of the vector.
         */
        float LengthSquared() const
        {
            return X * X + Y * Y;
        }

        /**
         * @brief Calculates the angle of the vector in radians.
         * @return The angle of the vector in radians.
         */
        float Angle() const
        {
            return atan2(Y, X);
        }

        /**
         * @brief Calculates the dot product with another vector.
         * @param other The other vector.
         * @return The dot product of the two vectors.
         */
        float Dot(const Vec2& other) const
        {
            return X * other.X + Y * other.Y;
        }

        /**
         * @brief Clamps the vector's components between a minimum and maximum value.
         * @param min The minimum value.
         * @param max The maximum value.
         */
        void Clamp(float min, float max)
        {
            X = std::max(min, std::min(X, max));
            Y = std::max(min, std::min(Y, max));
        }

        /**
         * @brief Floors the vector's components to the nearest lower integer values.
         */
        void Floor()
        {
            X = floor(X);
            Y = floor(Y);
        }

        // Operator overloads for arithmetic operations
        Vec2& operator+=(const Vec2& other)
        {
            X += other.X;
            Y += other.Y;
            return *this;
        }

        Vec2& operator+=(float scalar)
        {
            X += scalar;
            Y += scalar;
            return *this;
        }

        Vec2& operator-=(const Vec2& other)
        {
            X -= other.X;
            Y -= other.Y;
            return *this;
        }

        Vec2& operator-=(float scalar)
        {
            X -= scalar;
            Y -= scalar;
            return *this;
        }

        Vec2& operator*=(const Vec2& other)
        {
            X *= other.X;
            Y *= other.Y;
            return *this;
        }

        Vec2& operator*=(float scalar)
        {
            X *= scalar;
            Y *= scalar;
            return *this;
        }

        Vec2& operator/=(const Vec2& other)
        {
            X /= other.X;
            Y /= other.Y;
            return *this;
        }

        Vec2& operator/=(float scalar)
        {
            X /= scalar;
            Y /= scalar;
            return *this;
        }

        Vec2 operator-() const
        {
            return Vec2(-X, -Y);
        }

        Vec2 operator+(const Vec2& other) const
        {
            return Vec2(X + other.X, Y + other.Y);
        }

        Vec2 operator-(const Vec2& other) const
        {
            return Vec2(X - other.X, Y - other.Y);
        }

        Vec2 operator*(const Vec2& other) const
        {
            return Vec2(X * other.X, Y * other.Y);
        }

        Vec2 operator*(float scalar) const
        {
            return Vec2(X * scalar, Y * scalar);
        }

        Vec2 operator/(const Vec2& other) const
        {
            return Vec2(X / other.X, Y / other.Y);
        }

        Vec2 operator/(float scalar) const
        {
            return Vec2(X / scalar, Y / scalar);
        }

        /**
         * @brief Sets the value of the vector along a specific axis.
         * @param axis The axis (X or Y).
         * @param value The value to set.
         */
        void SetValueAlongAxis(Axis axis, float value)
        {
            if (axis == Axis::X)
            {
                X = value;
            }
            else
            {
                Y = value;
            }
        }

        /**
         * @brief Gets the value of the vector along a specific axis.
         * @param axis The axis (X or Y).
         * @return The value along the specified axis.
         */
        float GetValueAlongAxis(Axis axis)
        {
            return (axis == Axis::X ? X : Y);
        }

        // Static utility methods
        /**
         * @brief Normalizes a given vector.
         * @param v The vector to normalize.
         * @return A normalized version of the input vector.
         */
        static Vec2 Normalize(const Vec2& v)
        {
            Vec2 result = v;
            result.Normalize();

            return result;
        }

        /**
         * @brief Rotates a given vector by a specified angle (in radians).
         * @param v The vector to rotate.
         * @param angle The angle to rotate by, in radians.
         * @return The rotated vector.
         */
        static Vec2 Rotate(const Vec2& v, float angle)
        {
            Vec2 result = v;
            result.Rotate(angle);

            return result;
        }

        /**
         * @brief Rotates a given vector around a pivot point by a specified angle (in radians).
         * @param v The vector to rotate.
         * @param angle The angle to rotate by, in radians.
         * @param pivot The pivot point for the rotation.
         * @return The rotated vector.
         */
        static Vec2 Rotate(const Vec2& v, float angle, const Vec2& pivot)
        {
            Vec2 result = v;
            result.Rotate(angle, pivot);

            return result;
        }

        /**
         * @brief Floors the components of a given vector to the nearest lower integer values.
         * @param v The vector to floor.
         * @return A vector with floored components.
         */
        static Vec2 Floor(const Vec2& v)
        {
            return Vec2(floor(v.X), floor(v.Y));
        }
    };
}