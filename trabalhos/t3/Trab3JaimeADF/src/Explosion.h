#include "Animation.h"

class Explosion : public Animation
{
public:
    Explosion(std::reference_wrapper<Scene> scene);

    void Initialize() override;
};