#pragma once

#include "VectorGraphic.h"
#include "Entity.h"
#include "PlayerCannon.h"
#include "Effect.h"

class Player : public Entity
{
public:
    Player(std::reference_wrapper<Scene> scene);

    void Initialize() override;

    void Update(float deltaTime) override;
    void Draw(DrawingContext &context) override;

    void Destroy() override;

    void ApplyEffect(Effect effect);

private:
    TesselatedGraphic m_Graphic;

    float m_RapidFireTimeRemaining;
    float m_InvincibleTimeRemaining;

    std::shared_ptr<PlayerCannon> m_Cannon;
};