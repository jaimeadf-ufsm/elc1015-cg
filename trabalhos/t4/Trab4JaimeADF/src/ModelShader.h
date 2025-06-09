#pragma once

#include "Shader.h"
#include "Sampler.h"

class ModelShader : public Shader
{
public:
    Matrix4x4 ProjectionMatrix;
    Matrix4x4 ViewMatrix;
    Matrix4x4 ModelMatrix;

    Sampler Texture;

    ModelShader();

    virtual VertexOutput ProcessVertex(const VertexInput& input) const override;
    virtual FragmentOutput ProcessFragment(const FragmentInput& input) const override;
};
