#include "ModelShader.h"

ModelShader::ModelShader() : ViewProjectionMatrix(), ModelMatrix()
{
}

VertexOutput ModelShader::ProcessVertex(const VertexInput& input) const
{
    VertexOutput output;

    Vector3 worldPosition = (ModelMatrix * Vector4(input.Position, 1.0f)).ToCartesianPosition();
    Vector3 worldNormal = (ModelMatrix * Vector4(input.Normal, 0.0f)).ToCartesianDirection().Normalize();

    output.ClipPosition = ViewProjectionMatrix * worldPosition;
    output.WorldPosition = worldPosition;
    output.UV = input.UV;
    output.Normal = worldNormal;

    return output;
}

FragmentOutput ModelShader::ProcessFragment(const FragmentInput& input) const
{
    FragmentOutput output;

    output.Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

    return output;
}
