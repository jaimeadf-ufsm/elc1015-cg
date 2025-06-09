#pragma once

#include "ModelShader.h"

class FlatShader : public ModelShader
{
public:
    FragmentOutput ProcessFragment(const FragmentInput& input) const override;
};
