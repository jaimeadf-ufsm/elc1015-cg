/**
 * @file Light.h
 * @brief Light class for managing OpenGL lighting properties
 */

#pragma once

#include <GL/glut.h>
#include "Vector.h"

/**
 * @class Light
 * @brief Manages OpenGL light sources with position, ambient, diffuse, and specular properties
 */
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

    void Enable()
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