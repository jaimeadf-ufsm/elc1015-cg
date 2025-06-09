#include "ModelShader.h"

ModelShader::ModelShader() : ProjectionMatrix(), ViewMatrix(), ModelMatrix()
{
}

VertexOutput ModelShader::ProcessVertex(const VertexInput& input) const
{
    VertexOutput output;

    Vector3 worldPosition = (ModelMatrix * Vector4(input.Position, 1.0f)).ToCartesianPosition();

    output.ClipPosition = ProjectionMatrix * ViewMatrix * ModelMatrix * Vector4(input.Position, 1.0f);
    output.WorldPosition = worldPosition;
    output.UV = input.UV;
    output.Normal = (ModelMatrix * Vector4(input.Normal, 0.0f)).ToCartesianDirection().Normalize();

    return output;
}

FragmentOutput ModelShader::ProcessFragment(const FragmentInput& input) const
{
    FragmentOutput output;

    output.Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

    return output;
}
