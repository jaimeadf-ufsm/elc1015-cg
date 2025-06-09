#include "WireframeShader.h"

FragmentOutput WireframeShader::ProcessFragment(const FragmentInput& input) const
{
    FragmentOutput output;

    float width = 0.01f;

    float closest = std::min({ input.Barycentric.X, input.Barycentric.Y, input.Barycentric.Z });
    float alpha = closest < width ? 1.0f : 0.0f;

    output.Color = Vector4(1.0f, 1.0f, 1.0f, alpha);

    return output;
}