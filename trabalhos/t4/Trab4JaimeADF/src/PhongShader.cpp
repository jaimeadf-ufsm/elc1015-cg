#include <cmath>

#include "PhongShader.h"

FragmentOutput PhongShader::ProcessFragment(const FragmentInput& input) const
{
    FragmentOutput output;

    Vector3 surfaceColor = Texture.Sample(input.UV);
    Vector3 normal = input.Normal.Normalize();
    Vector3 lightDirection = (LightPosition - input.WorldPosition).Normalize();
    Vector3 viewDirection = (CameraPosition - input.WorldPosition).Normalize();

    Vector3 ambient = surfaceColor * Material.AmbientStrength;

    float diffuseFactor = std::max(normal.Dot(lightDirection), 0.0f);
    Vector3 diffuse = surfaceColor * Material.DiffuseStrength * diffuseFactor;

    Vector3 reflectDirection = (normal * (2.0f * normal.Dot(lightDirection)) - lightDirection).Normalize();
    float specularFactor = std::pow(std::max(viewDirection.Dot(reflectDirection), 0.0f), Material.Shininess);
    Vector3 specular = Vector3(1.0f, 1.0f, 1.0f) * Material.SpecularStrength * specularFactor;

    Vector3 finalColor = ambient + diffuse + specular;

    output.Color = Vector4(finalColor.X, finalColor.Y, finalColor.Z, 1.0f);

    return output;
}