#pragma once

#include "Enemy.h"
#include "VectorGraphic.h"

class Box : public Enemy
{
public:
    Box(std::reference_wrapper<Game> game);

    void Initialize() override;

    void Update(float deltaTime) override;
    void Draw(DrawingContext &context) override;

private:
    TesselatedGraphic m_Graphic;
};