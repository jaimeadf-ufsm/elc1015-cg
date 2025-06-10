#include <cmath>

#include "Camera.h"
#include "Math.h"

Camera::Camera(float yaw, float pitch, const Vector3& position) : m_Yaw(yaw), m_Pitch(pitch), m_Position(position), m_Zoom(1.0f)
{
    Refresh();
}

void Camera::SetPosition(const Vector3& position)
{
    m_Position = position;
    Refresh();
}

const Vector3& Camera::GetPosition() const
{
    return m_Position;
}

void Camera::SetYaw(float yaw)
{
    while (yaw > MATH_PI) yaw -= MATH_TAU;
    while (yaw < -MATH_PI) yaw += MATH_TAU;
    
    m_Yaw = yaw;
    Refresh();
}

void Camera::SetPitch(float pitch)
{
    m_Pitch = Clamp(pitch, -MATH_PI / 2.0f + 0.05f, MATH_PI / 2.0f - 0.05f);
    Refresh();
}

float Camera::GetYaw() const
{
    return m_Yaw;
}

float Camera::GetPitch() const
{
    return m_Pitch;
}

void Camera::SetZoom(float zoom)
{
    m_Zoom = Clamp(zoom, 0.1f, 10.0f);
}

float Camera::GetZoom() const
{
    return m_Zoom;
}

Vector3 Camera::GetFront() const
{
    return m_Front;
}

Vector3 Camera::GetUp() const
{
    return m_Up;
}

Vector3 Camera::GetRight() const
{
    return m_Right;
}

Matrix4x4 Camera::GetViewMatrix() const
{
    return Matrix4x4::LookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::Reset()
{
    m_Yaw = -MATH_PI / 2.0f + 0.05f;
    m_Pitch = 0.0f;
    m_Position = Vector3(0.0f, 0.0f, 0.0f);
    m_Zoom = 1.0f;
    Refresh();
}

void Camera::Refresh()
{
    const Vector3 worldUp(0.0f, 1.0f, 0.0f);

    m_Front.X = std::cos(m_Yaw) * std::cos(m_Pitch);
    m_Front.Y = std::sin(m_Pitch);
    m_Front.Z = std::sin(m_Yaw) * std::cos(m_Pitch);
    
    m_Front = m_Front.Normalize();
    m_Right = m_Front.Cross(worldUp).Normalize();
    m_Up = m_Right.Cross(m_Front).Normalize();
}
