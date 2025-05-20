#pragma once

#include "Entity.h"

class Enemy : public Entity
{
public:
    Enemy(std::reference_wrapper<Scene> scene);

    virtual void Initialize();

    virtual void Update(float deltaTime) override;
    virtual void Draw(DrawingContext& context) override;

    virtual void Collide(const Contact& contact) override;

    virtual void Destroy() override;

private:
    std::unordered_set<std::shared_ptr<GameObject>> m_AttackedEntities;
};