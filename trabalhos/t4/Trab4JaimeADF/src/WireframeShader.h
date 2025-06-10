/**
 * @file WireframeShader.h
 * @brief Wireframe shader implementation for line-based rendering
 */

#pragma once

#include "ModelShader.h"

/**
 * @class WireframeShader
 * @brief Shader that renders models in wireframe mode showing only edges
 */
class WireframeShader : public ModelShader
{
public:
    FragmentOutput ProcessFragment(const FragmentInput& input) const override;
};
