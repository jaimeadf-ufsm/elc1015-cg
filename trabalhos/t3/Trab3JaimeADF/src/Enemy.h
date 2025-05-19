#pragma once

#include "Entity.h"

class Enemy : public Entity
{
public:
    Enemy(std::reference_wrapper<Game> game);

    virtual void Initialize();

    virtual void Update(float deltaTime) override;
    virtual void Draw(DrawingContext& context) override;

    virtual void Collide(const Contact& contact) override;

    virtual void Destroy() override;
};