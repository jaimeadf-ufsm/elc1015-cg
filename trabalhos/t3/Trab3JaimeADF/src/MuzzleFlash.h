#pragma once

#include "Animation.h"

class MuzzleFlash : public Animation
{
public:
    MuzzleFlash(std::reference_wrapper<Game> game);

    void Initialize() override;
};