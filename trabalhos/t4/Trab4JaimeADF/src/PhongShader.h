/**
 * @file PhongShader.h
 * @brief Phong shading implementation for realistic lighting
 */

#include "ModelShader.h"

/**
 * @class PhongShader
 * @brief Shader implementing Phong lighting model with specular highlights
 */
class PhongShader : public ModelShader
{
public:
    Vector3 LightPosition;

    FragmentOutput ProcessFragment(const FragmentInput& input) const override;
};
