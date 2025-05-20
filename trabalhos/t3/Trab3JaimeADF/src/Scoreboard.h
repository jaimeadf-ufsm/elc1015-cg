#pragma once

#include "GameObject.h"
#include "Track.h"

class Scoreboard : public GameObject
{
public:
    Scoreboard(std::reference_wrapper<Scene> scene);

    void Initialize() override;

    void Update(float deltaTime) override;
    void Draw(DrawingContext &context) override;
};