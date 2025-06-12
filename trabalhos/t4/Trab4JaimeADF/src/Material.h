/**
 * @file Material.h
 * @brief Material properties for Phong lighting model
 */

#pragma once

#include "Vector3.h"

/**
 * @class Material
 * @brief Encapsulates material properties for realistic lighting calculations
 */
class Material
{
public:
    float AmbientStrength;
    float DiffuseStrength;
    float SpecularStrength;
    float Shininess;

    Material();
    Material(float ambientStrength, float diffuseStrength, float specularStrength, float shininess);

    static Material CreateRubber();
    static Material CreatePlastic();
    static Material CreateMetal();
};
