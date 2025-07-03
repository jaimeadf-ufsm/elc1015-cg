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

    Vector3 Right;
    Vector3 Forward;
    
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

    void MoveForward()
    {
        MoveForward(Speed * Time::DeltaTime);
    }

    void MoveBackward()
    {
        MoveBackward(Speed * Time::DeltaTime);
    }

    void MoveRight()
    {
        MoveRight(Speed * Time::DeltaTime);
    }

    void MoveLeft()
    {
        MoveLeft(Speed * Time::DeltaTime);
    }

    void MoveForward(float distance)
    {
        Position = Position + Forward * distance;
        Target = Target + Forward * distance;
    }

    void MoveBackward(float distance)
    {
        Position = Position - Forward * distance;
        Target = Target - Forward * distance;
    }

    void MoveRight(float distance)
    {
        Position = Position + Right * distance;
        Target = Target + Right * distance;
    }

    void MoveLeft(float distance)
    {
        Position = Position - Right * distance;
        Target = Target - Right * distance;
    }

    void ProcessKeyboard(char key)
    {
        float velocity = Speed * Time::DeltaTime;

        switch (key)
        {
            case 'w':
                MoveForward(velocity);
                break;
            case 's':
                MoveBackward(velocity);
                break;
            case 'a':
                MoveLeft(velocity);
                break;
            case 'd':
                MoveRight(velocity);
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
        
        Forward = front.Normalize();
        Target = Position + Forward;

        Right = Forward.Cross(Up).Normalize();
    }
};