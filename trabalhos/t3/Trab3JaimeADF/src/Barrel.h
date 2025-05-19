#pragma once

#include "Entity.h"
#include "VectorGraphic.h"

class Barrel : public Entity
{
public:
    Barrel(std::reference_wrapper<Game> game);

    void Initialize();

    void Update(float deltaTime);
    void Draw(DrawingContext &context);

private:
    TesselatedGraphic m_Graphic;
};