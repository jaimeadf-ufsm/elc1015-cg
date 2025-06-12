/**
 * @file ModelShader.h
 * @brief Base shader class for 3D model rendering with transformations
 */

#pragma once

#include "Shader.h"
#include "Sampler.h"

/**
 * @class ModelShader
 * @brief Base shader class for 3D models with MVP matrices and texture support
 */
class ModelShader : public Shader
{
public:
    Matrix4x4 ViewProjectionMatrix;
    Matrix4x4 ModelMatrix;

    Sampler Texture;

    ModelShader();

    virtual VertexOutput ProcessVertex(const VertexInput& input) const override;
    virtual FragmentOutput ProcessFragment(const FragmentInput& input) const override;
};
