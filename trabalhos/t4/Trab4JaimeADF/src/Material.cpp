#include "Material.h"

Material::Material() : AmbientStrength(0.2f), DiffuseStrength(0.8f), SpecularStrength(0.5f), Shininess(32.0f)
{
}

Material::Material(float ambientStrength, float diffuseStrength, float specularStrength, float shininess) :
    AmbientStrength(ambientStrength),
    DiffuseStrength(diffuseStrength),
    SpecularStrength(specularStrength),
    Shininess(shininess)
{
}

Material Material::CreateRubber()
{
    return Material(
        0.3f,
        0.8f,
        0.1f,
        2.0f
    );
}

Material Material::CreatePlastic()
{
    return Material(
        0.2f,
        0.7f,
        0.6f,
        32.0f
    );
}

Material Material::CreateMetal()
{
    return Material(
        0.1f,
        0.6f,
        1.0f,
        128.0f
    );
}
