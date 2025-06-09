#include "ModelShader.h"

class PhongShader : public ModelShader
{
public:
    Vector3 LightPosition;

    FragmentOutput ProcessFragment(const FragmentInput& input) const override;
};
