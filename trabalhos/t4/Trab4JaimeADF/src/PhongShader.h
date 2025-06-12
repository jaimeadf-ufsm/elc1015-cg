/**
 * @file PhongShader.h
 * @brief Phong shading implementation for realistic lighting
 */

#include "ModelShader.h"
#include "Material.h"

/**
 * @class PhongShader
 * @brief Shader implementing Phong lighting model with specular highlights
 */
class PhongShader : public ModelShader
{
public:
    Vector3 LightPosition;
    Vector3 CameraPosition;
    Material Material;

    FragmentOutput ProcessFragment(const FragmentInput& input) const override;
};
