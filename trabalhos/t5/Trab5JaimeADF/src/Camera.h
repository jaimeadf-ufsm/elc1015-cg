/**
 * @file Camera.h
 * @brief Camera class for 3D scene navigation and view control
 */

#pragma once

#include <GL/glut.h>
#include "Mathematics.h"
#include "Vector.h"
#include "Time.h"

/**
 * @class Camera
 * @brief Manages 3D camera position, orientation, and movement controls
 */
class Camera
{
public:
    Vector3 Position;
    Vector3 Target;
    Vector3 Up;
    
    float Yaw;
    float Pitch;
    float Speed;
    float Sensitivity;

    Camera(Vector3 position = Vector3(0, 0, 10), Vector3 target = Vector3(0, 0, 0), Vector3 up = Vector3(0, 1, 0))
        : Position(position), Target(target), Up(up), Yaw(-90.0f), Pitch(0.0f), Speed(5.0f), Sensitivity(0.1f)
    {
        UpdateVectors();
    }

    void Apply()
    {
        gluLookAt(
            Position.X,
            Position.Y,
            Position.Z,
            Target.X,
            Target.Y,
            Target.Z,
            Up.X,
            Up.Y,
            Up.Z
        );
    }

    void ProcessKeyboard(char key)
    {
        float velocity = Speed * Time::DeltaTime;
        Vector3 forward = (Target - Position).Normalize();
        Vector3 right = forward.Cross(Up).Normalize();

        switch (key)
        {
            case 'w':
                Position = Position + forward * velocity;
                Target = Target + forward * velocity;
                break;
            case 's':
                Position = Position - forward * velocity;
                Target = Target - forward * velocity;
                break;
            case 'a':
                Position = Position - right * velocity;
                Target = Target - right * velocity;
                break;
            case 'd':
                Position = Position + right * velocity;
                Target = Target + right * velocity;
                break;
        }
    }

    void ProcessMouseMovement(float xOffset, float yOffset)
    {
        xOffset *= Sensitivity;
        yOffset *= Sensitivity;

        Yaw += xOffset;
        Pitch += yOffset;

        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;

        UpdateVectors();
    }

private:
    void UpdateVectors()
    {
        Vector3 front;
        front.X = cosf(Yaw * M_PI / 180.0f) * cosf(Pitch * M_PI / 180.0f);
        front.Y = sinf(Pitch * M_PI / 180.0f);
        front.Z = sinf(Yaw * M_PI / 180.0f) * cosf(Pitch * M_PI / 180.0f);
        
        Vector3 forward = front.Normalize();
        Target = Position + forward;
    }
};