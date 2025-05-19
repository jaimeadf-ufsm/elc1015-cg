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

    void Collide(const Contact& contact) override;

private:
    TesselatedGraphic m_Graphic;
    std::unordered_set<std::shared_ptr<GameObject>> m_AttackedEntities;
};