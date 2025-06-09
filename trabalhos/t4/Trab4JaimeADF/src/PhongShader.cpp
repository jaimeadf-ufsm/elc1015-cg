#include "PhongShader.h"

FragmentOutput PhongShader::ProcessFragment(const FragmentInput& input) const
{
    FragmentOutput output;

    Vector3 surfaceColor = Texture.Sample(input.UV);

    Vector3 lightDirection = (LightPosition - input.WorldPosition).Normalize();
    Vector3 normal = input.Normal.Normalize();

    float diffuse = std::max(normal.Dot(lightDirection), 0.0f);

    Vector3 diffuseColor = surfaceColor * diffuse;
    Vector3 ambientColor = surfaceColor * 0.1f;

    Vector3 finalColor = ambientColor + diffuseColor;

    output.Color = Vector4(finalColor.X, finalColor.Y, finalColor.Z, 1.0f);

    return output;
}