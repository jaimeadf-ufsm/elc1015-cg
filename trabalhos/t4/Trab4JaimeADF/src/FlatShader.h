/**
 * @file FlatShader.h
 * @brief Flat shading implementation for 3D models
 */

#pragma once

#include "ModelShader.h"

/**
 * @class FlatShader
 * @brief Shader that provides flat shading without lighting calculations
 */
class FlatShader : public ModelShader
{
public:
    FragmentOutput ProcessFragment(const FragmentInput& input) const override;
};
