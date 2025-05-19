#pragma once

#include "Entity.h"
#include "VectorGraphic.h"

class Box : public Entity
{
public:
    Box(std::reference_wrapper<Game> game);

    void Initialize();

    void Update(float deltaTime);
    void Draw(DrawingContext &context);

private:
    TesselatedGraphic m_Graphic;
};