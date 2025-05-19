#pragma once

#include "Enemy.h"
#include "VectorGraphic.h"

class Shuriken : public Enemy
{
public:
    Shuriken(std::reference_wrapper<Game> game);

    void Initialize();

    void Update(float deltaTime) override;
    void Draw(DrawingContext &context) override;

private:
    TesselatedGraphic m_Graphic;
};