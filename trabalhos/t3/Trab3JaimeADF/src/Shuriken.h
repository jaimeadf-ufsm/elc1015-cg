#pragma once

#include "Entity.h"
#include "VectorGraphic.h"

class Shuriken : public Entity
{
public:
    Shuriken(std::reference_wrapper<Game> game);

    void Initialize();

    void Update(float deltaTime);
    void Draw(DrawingContext &context);
    void Collide(const Contact& contact);

private:
    TesselatedGraphic m_Graphic;
    std::unordered_set<std::shared_ptr<GameObject>> m_AttackedEntities;
};