#pragma once

#include <GL/glut.h>
#include "Vector.h"

class Light
{
public:
    GLenum LightID;
    Vector4 Position;
    Vector4 Ambient;
    Vector4 Diffuse;
    Vector4 Specular;

    Light(GLenum lightId = GL_LIGHT0)
        : LightID(lightId),
          Position(0.0f, 0.0f, 0.0f, 1.0f),
          Ambient(0.2f, 0.2f, 0.2f, 1.0f),
          Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
          Specular(1.0f, 1.0f, 1.0f, 1.0f)
    {
    }

    void SetPosition(Vector3 pos)
    {
        Position = Vector4(pos.X, pos.Y, pos.Z, 1.0f);
    }

    void SetDirectional(Vector3 direction)
    {
        Position = Vector4(direction.X, direction.Y, direction.Z, 0.0f);
    }

    void Apply()
    {
        glEnable(LightID);
        glLightfv(LightID, GL_POSITION, Position.ToArray());
        glLightfv(LightID, GL_AMBIENT, Ambient.ToArray());
        glLightfv(LightID, GL_DIFFUSE, Diffuse.ToArray());
        glLightfv(LightID, GL_SPECULAR, Specular.ToArray());
    }

    void Disable()
    {
        glDisable(LightID);
    }
};