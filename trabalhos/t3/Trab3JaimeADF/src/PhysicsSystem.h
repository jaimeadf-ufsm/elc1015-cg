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
    PhysicsSystem(std::reference_wrapper<Scene> scene);
  
    void Simulate(float deltaTime);

    void OverlapObjectsWithBox(std::vector<std::shared_ptr<GameObject>>& overlapingObjects, const Vector2& start, const Vector2& end);
    void OverlapObjectsWithPolygon(std::vector<std::shared_ptr<GameObject>>& overlapingObjects, const std::vector<Vector2>& polygon);

private:
    std::reference_wrapper<Scene> m_Scene;

    std::vector<Vector2> m_TransformedPolygonA;
    std::vector<Vector2> m_TransformedPolygonB;

    std::vector<Collision> m_Collisions;

    void Integrate(float deltaTime);
    void SolveCollisions(float deltaTime);
    void FireContactCallbacks();

    void SolveCollision(Collision& collision);
};