#pragma once

#include "ModelShader.h"

class WireframeShader : public ModelShader
{
public:
    FragmentOutput ProcessFragment(const FragmentInput& input) const override;
};
