#pragma once

#include "GameObject.h"

class TrackBoundary : public GameObject
{
public:
    TrackBoundary(std::reference_wrapper<Game> game);

    void Initialize() override;

    void Update(float deltaTime) override;
    void Draw(DrawingContext &context) override;
};