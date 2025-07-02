#pragma once

#include <GL/glut.h>
#include <memory>

#include "Transform.h"
#include "Vector.h"
#include "Model.h"
#include "Time.h"

class Body
{
public:
    Transform Transform;
    std::shared_ptr<::Model> Model;

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