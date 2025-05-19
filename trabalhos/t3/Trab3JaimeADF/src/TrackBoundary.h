#pragma once

#include "GameObject.h"

class TrackBoundary : public GameObject
{
public:
    TrackBoundary(std::reference_wrapper<Game> game);

    void Initialize();

    void Update(float deltaTime);
    void Draw(DrawingContext &context);
};