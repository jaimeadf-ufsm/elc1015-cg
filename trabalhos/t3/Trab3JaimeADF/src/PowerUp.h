#pragma once

#include "GameObject.h"
#include "Effect.h"
#include "VectorGraphic.h"

class PowerUp : public GameObject
{
public:
    PowerUp(std::reference_wrapper<Game> game);

    void Initialize() override;

    void Update(float deltaTime) override;
    void Draw(DrawingContext& context) override;

    void Collide(const Contact& contact) override;

private:
    TesselatedGraphic m_Graphic;

    float m_Animation;
    Effect m_Effect;
};