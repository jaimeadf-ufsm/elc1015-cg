#pragma once

#include "GameObject.h"
#include "Health.h"

class Entity : public GameObject
{
public:
    std::shared_ptr<Health> Health;

    Entity(std::reference_wrapper<Game> game);

    virtual void Initialize();

    virtual void Update(float deltaTime) override;
    virtual void Draw(DrawingContext& context) override;

    virtual void Collide(const Contact& contact) override;

    virtual void Destroy() override;
};