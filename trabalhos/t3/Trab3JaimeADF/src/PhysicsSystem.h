#pragma once

#include <unordered_set>

#include "GameObject.h"
#include "Contact.h"

struct Collision
{
    float Duration;

    float Depth;
    Vector2 Normal;

    std::shared_ptr<GameObject> ObjectA;
    std::shared_ptr<GameObject> ObjectB;

    std::shared_ptr<Collider> ColliderA;
    std::shared_ptr<Collider> ColliderB;
};

class PhysicsSystem
{
private:

public:
    PhysicsSystem(std::reference_wrapper<Game> game);

    void Simulate(float deltaTime);

private:
    std::reference_wrapper<Game> m_Game;

    std::vector<Vector2> m_TransformedPolygonA;
    std::vector<Vector2> m_TransformedPolygonB;

    std::vector<Collision> m_Collisions;

    void Integrate(float deltaTime);
    void SolveCollisions(float deltaTime);
    void FireContactCallbacks();

    void SolveCollision(Collision& collision);
};