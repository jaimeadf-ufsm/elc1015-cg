/**
 * @file Camera.h
 * @brief 3D camera class for view transformation and navigation
 */

#pragma once

#include "Matrix4x4.h"
#include "Math.h"

/**
 * @class Camera
 * @brief 3D camera with position, orientation, and view matrix generation
 */
class Camera
{
public:
    Camera(float yaw = -MATH_PI / 2.0f + 0.05f, float pitch = 0.0f, const Vector3& position = Vector3(0.0f, 0.0f, 0.0f));

    void SetYaw(float yaw);
    void SetPitch(float pitch);

    float GetYaw() const;
    float GetPitch() const;

    void SetPosition(const Vector3& position);
    const Vector3& GetPosition() const;

    void SetZoom(float zoom);
    float GetZoom() const;

    Vector3 GetFront() const;
    Vector3 GetUp() const;
    Vector3 GetRight() const;

    Matrix4x4 GetViewMatrix() const;

    void Reset();

private:
    float m_Yaw;
    float m_Pitch;

    Vector3 m_Position;
    float m_Zoom;

    Vector3 m_Front;
    Vector3 m_Up;
    Vector3 m_Right;

    void Refresh();
};