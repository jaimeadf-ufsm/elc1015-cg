/**
 * @file Body.h
 * @brief Body class for representing physical objects in 3D space
 */

#pragma once

#include <GL/glut.h>
#include <memory>

#include "Transform.h"
#include "Vector.h"
#include "Model.h"
#include "Time.h"

/**
 * @class Body
 * @brief Represents a game object with transform, model, and angular velocity properties for asteroid and sun simulation
 */
class Body
{
public:
    std::shared_ptr<::Model> Model;

    Transform Transform;
    Vector3 AngularVelocity;

    Body() {}

    void Update()
    {
        Transform.Rotation += AngularVelocity * Time::DeltaTime;
    }

    void Render(Vector3 cameraPosition) const
    {
        if (!Model) return;

        glPushMatrix();

        Transform.Apply();

        Vector3 distance = Transform.Position - cameraPosition;
        float distanceLength = distance.Length();

        Model->Render(distanceLength);

        glPopMatrix();
    }
};