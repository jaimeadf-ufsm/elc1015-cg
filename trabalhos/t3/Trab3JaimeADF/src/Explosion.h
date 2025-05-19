#include "Animation.h"

class Explosion : public Animation
{
public:
    Explosion(std::reference_wrapper<Game> game);

    void Initialize() override;
};