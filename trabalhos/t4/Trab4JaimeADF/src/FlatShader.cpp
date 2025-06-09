#include "FlatShader.h"

FragmentOutput FlatShader::ProcessFragment(const FragmentInput& input) const
{
    FragmentOutput output;

    Vector3 surfaceColor = Texture.Sample(input.UV);

    output.Color = Vector4(surfaceColor.X, surfaceColor.Y, surfaceColor.Z, 1.0f);

    return output;
}

