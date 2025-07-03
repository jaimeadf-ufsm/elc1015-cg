/**
 * @file Transform.h
 * @brief Transform class for 3D object positioning, rotation, and scaling
 */

#pragma once

#include <GL/glut.h>
#include "Vector.h"

/**
 * @class Transform
 * @brief Handles 3D transformation operations including position, rotation, and scale
 */
class Transform
{
public:
    Vector3 Position;
    Vector3 Rotation;
    Vector3 Scale;

    Transform(
        Vector3 position = Vector3(0, 0, 0), 
        Vector3 rotation = Vector3(0, 0, 0), 
        Vector3 scale = Vector3(1, 1, 1)) :
        Position(position),
        Rotation(rotation),
        Scale(scale) {}

    void Apply() const
    {
        glTranslatef(Position.X, Position.Y, Position.Z);
        glRotatef(Rotation.X, 1.0f, 0.0f, 0.0f);
        glRotatef(Rotation.Y, 0.0f, 1.0f, 0.0f);
        glRotatef(Rotation.Z, 0.0f, 0.0f, 1.0f);
        glScalef(Scale.X, Scale.Y, Scale.Z);
    }
};