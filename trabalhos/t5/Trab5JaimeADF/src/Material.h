#pragma once

#include <GL/glut.h>
#include "Vector.h"

class Material
{
public:
    Vector4 Ambient;
    Vector4 Diffuse;
    Vector4 Specular;
    Vector4 Emission;
    float Shininess;

    Material(
        Vector4 ambient = Vector4(0.2f, 0.2f, 0.2f, 1.0f),
        Vector4 diffuse = Vector4(0.8f, 0.8f, 0.8f, 1.0f),
        Vector4 specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f),
        Vector4 emission = Vector4(0.0f, 0.0f, 0.0f, 1.0f),
        float shininess = 32.0f)
        : Ambient(ambient),
          Diffuse(diffuse),
          Specular(specular),
          Emission(emission),
          Shininess(shininess)
    {
    }

    void Apply() const
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ambient.ToArray());
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Diffuse.ToArray());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Specular.ToArray());
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission.ToArray());
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Shininess);
    }
};