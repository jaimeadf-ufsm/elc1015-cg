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

    void Apply() const
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ambient.ToArray());
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Diffuse.ToArray());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Specular.ToArray());
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission.ToArray());
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Shininess);
    }
};