#pragma once

#include "GameObject.h"
#include "Track.h"

class GameOver : public GameObject
{
public:
    GameOver(std::reference_wrapper<Scene> scene);

    void Initialize() override;

    void Update(float deltaTime) override;
    void Draw(DrawingContext &context) override;

    bool IsOver() const;
private:
    bool m_Over = false;

    bool IsPlayerAlive();
};