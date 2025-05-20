#pragma once

#include "GameObject.h"
#include "Track.h"

class Score : public GameObject
{
public:
    Score(std::reference_wrapper<Scene> scene);

    void Initialize() override;

    void Update(float deltaTime) override;
    void Draw(DrawingContext &context) override;

    void IncrementScore(int amount);

    int GetValue() const;
private:
    int m_Score;
};