#pragma once

#include "Enemy.h"
#include "VectorGraphic.h"

class Barrel : public Enemy
{
public:
    Barrel(std::reference_wrapper<Game> game);

    void Initialize();

    void Update(float deltaTime);
    void Draw(DrawingContext &context);

private:
    TesselatedGraphic m_Graphic;
};