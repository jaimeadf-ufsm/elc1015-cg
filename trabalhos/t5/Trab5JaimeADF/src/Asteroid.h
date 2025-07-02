#pragma once

#include <memory>
#include <GL/glut.h>
#include "Vector.h"
#include "Model.h"

class Asteroid
{
public:
    Vector3 Position;
    Vector3 Rotation;
    Vector3 RotationSpeed;
    Vector3 Scale;
    std::shared_ptr<Model> AsteroidModel;

    Asteroid(Vector3 position, std::shared_ptr<Model> model)
        : Position(position), 
          Rotation(0, 0, 0),
          RotationSpeed((rand() % 100 - 50) / 10.0f, (rand() % 100 - 50) / 10.0f, (rand() % 100 - 50) / 10.0f),
          Scale(0.5f + (rand() % 100) / 200.0f, 0.5f + (rand() % 100) / 200.0f, 0.5f + (rand() % 100) / 200.0f),
          AsteroidModel(model)
    {
    }

    void Update(float deltaTime)
    {
        Rotation.X += RotationSpeed.X * deltaTime;
        Rotation.Y += RotationSpeed.Y * deltaTime;
        Rotation.Z += RotationSpeed.Z * deltaTime;
    }

    void Render(Vector3 cameraPosition) const
    {
        if (!AsteroidModel) return;

        glPushMatrix();

        // Transform to asteroid position
        glTranslatef(Position.X, Position.Y, Position.Z);
        glRotatef(Rotation.X, 1, 0, 0);
        glRotatef(Rotation.Y, 0, 1, 0);
        glRotatef(Rotation.Z, 0, 0, 1);
        glScalef(Scale.X, Scale.Y, Scale.Z);

        // Calculate distance for LOD
        Vector3 distance = Position - cameraPosition;
        float distanceLength = distance.Length();

        // Render with appropriate LOD
        AsteroidModel->Render(distanceLength);

        glPopMatrix();
    }
};