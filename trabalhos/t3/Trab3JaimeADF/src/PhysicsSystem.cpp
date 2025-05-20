#include <limits>
#include <math.h>

#include "PhysicsSystem.h"
#include "Scene.h"
#include "GameObject.h"

struct PolygonIntersectionResult
{
    float Depth;
    Vector2 Normal;
};

struct PolygonProjectionResult
{
    float Minimum;
    float Maximum;
};

Vector2 EstimatePolygonCenter(const std::vector<Vector2>& polygon)
{
    Vector2 centroid;

    for (const Vector2& vertex : polygon)
    {
        centroid += vertex;
    }

    centroid /= polygon.size();

    return centroid;
}

void ProjectPolygonAlongAxis(
    PolygonProjectionResult& result,
    const std::vector<Vector2>& polygon,
    Vector2 axis
)
{
    result.Minimum = std::numeric_limits<float>::max();
    result.Maximum = std::numeric_limits<float>::lowest();

    for (const Vector2& vertex : polygon)
    {
        float projection = vertex.ProjectOnto(axis);

        result.Minimum = std::min(result.Minimum, projection);
        result.Maximum = std::max(result.Maximum, projection);
    }
}

bool EvaluateIntersectionAlongPolygonEdges(
    PolygonIntersectionResult& result,
    const std::vector<Vector2>& edgePolygon,
    const std::vector<Vector2>& otherPolygon
)
{
    for (size_t i = 0; i < edgePolygon.size(); i++)
    {
        Vector2 edge = edgePolygon[(i + 1) % edgePolygon.size()] - edgePolygon[i];
        Vector2 normal = Vector2(-edge.Y, edge.X);

        normal = normal.Normalized();

        PolygonProjectionResult projectionA;
        PolygonProjectionResult projectionB;

        ProjectPolygonAlongAxis(projectionA, edgePolygon, normal);
        ProjectPolygonAlongAxis(projectionB, otherPolygon, normal);

        if (projectionA.Minimum >= projectionB.Maximum || projectionB.Minimum >= projectionA.Maximum)
        {
            return false;
        }

        float depth = std::min(projectionA.Maximum, projectionB.Maximum) - std::max(projectionA.Minimum, projectionB.Minimum);

        if (depth <= result.Depth)
        {
            result.Depth = depth;
            result.Normal = normal;
        }
    }

    return true;
}

bool IntersectPolygons(
    PolygonIntersectionResult& result,
    const std::vector<Vector2>& polygonA,
    const std::vector<Vector2>& polygonB
)
{
    result.Depth = std::numeric_limits<float>::max();
    result.Normal = Vector2();

    if (!EvaluateIntersectionAlongPolygonEdges(result, polygonA, polygonB))
    {
        return false;
    }

    if (!EvaluateIntersectionAlongPolygonEdges(result, polygonB, polygonA))
    {
        return false;
    }

    Vector2 centerA = EstimatePolygonCenter(polygonA);
    Vector2 centerB = EstimatePolygonCenter(polygonB);

    Vector2 direction = centerB - centerA;

    if (direction.Dot(result.Normal) < 0.0f)
    {
        result.Normal = -result.Normal;
    }

    return true;
}

PhysicsSystem::PhysicsSystem(std::reference_wrapper<Scene> scene) : m_Scene(scene)
{
}

void PhysicsSystem::Simulate(float deltaTime)
{
    Integrate(deltaTime);
    SolveCollisions(deltaTime);
    FireContactCallbacks();
}

void PhysicsSystem::OverlapObjectsWithBox(std::vector<std::shared_ptr<GameObject>>& objects, const Vector2& start, const Vector2& end)
{
    OverlapObjectsWithPolygon(
        objects,
        {
            Vector2(start.X, start.Y),
            Vector2(end.X, start.Y),
            Vector2(end.X, end.Y),
            Vector2(start.X, end.Y)
        }
    );
}

void PhysicsSystem::OverlapObjectsWithPolygon(std::vector<std::shared_ptr<GameObject>>& overlapingObjects, const std::vector<Vector2>& polygon)
{
    Scene& scene = m_Scene.get();

    overlapingObjects.clear();

    for (std::shared_ptr<GameObject>& object : scene.GetObjects())
    {
        for (std::shared_ptr<Collider> collider : object->Colliders)
        {
            object->Transform->Apply(collider->GetPolygon(), m_TransformedPolygonA);

            PolygonIntersectionResult intersection;

            if (IntersectPolygons(intersection, m_TransformedPolygonA, polygon))
            {
                overlapingObjects.push_back(object);
                break;
            }
        }
    }
}

void PhysicsSystem::Integrate(float deltaTime)
{
    Scene& scene = m_Scene.get();

    for (std::shared_ptr<GameObject>& object : scene.GetObjects())
    {
        Vector2 linearOffset = object->Body->GetLinearVelocity() * deltaTime;
        float angularOffset = object->Body->GetAngularVelocity() * deltaTime;

        object->Transform->SetPosition(object->Transform->GetPosition() + linearOffset);
        object->Transform->SetRotation(object->Transform->GetRotation() + angularOffset);
    }
}

void PhysicsSystem::SolveCollisions(float deltaTime)
{
    Scene& scene = m_Scene.get();

    std::vector<std::shared_ptr<GameObject>>& objects = scene.GetObjects();

    m_Collisions.clear();

    for (size_t i = 0; i < objects.size(); i++)
    {
        std::shared_ptr<GameObject> objectA = objects[i];

        for (std::shared_ptr<Collider> colliderA : objectA->Colliders)
        {
            objectA->Transform->Apply(colliderA->GetPolygon(), m_TransformedPolygonA);

            for (size_t j = i + 1; j < objects.size(); j++)
            {
                std::shared_ptr<GameObject> objectB = objects[j];

                for (std::shared_ptr<Collider> colliderB : objectB->Colliders)
                {
                    PolygonIntersectionResult intersection;

                    objectB->Transform->Apply(colliderB->GetPolygon(), m_TransformedPolygonB);

                    if (IntersectPolygons(intersection, m_TransformedPolygonA, m_TransformedPolygonB))
                    {
                        Collision collision = {
                            .Duration = deltaTime,
                            .Depth = intersection.Depth,
                            .Normal = intersection.Normal,
                            .ObjectA = objectA,
                            .ObjectB = objectB,
                            .ColliderA = colliderA,
                            .ColliderB = colliderB
                        };

                        SolveCollision(collision);

                        m_Collisions.emplace_back(collision);
                    }
                }
            }
        }
    }
}

void PhysicsSystem::FireContactCallbacks()
{
    for (const Collision& collision : m_Collisions)
    {
        Contact contactAtoB = {
            .Duration = collision.Duration,
            .Collider = collision.ColliderA,
            .OtherObject = collision.ObjectB,
            .OtherCollider = collision.ColliderB
        };

        Contact contactBtoA = {
            .Duration = collision.Duration,
            .Collider = collision.ColliderB,
            .OtherObject = collision.ObjectA,
            .OtherCollider = collision.ColliderA
        };

        collision.ObjectA->Collide(contactAtoB);
        collision.ObjectB->Collide(contactBtoA);
    }
}

void PhysicsSystem::SolveCollision(Collision& collision)
{
    float depth = collision.Depth;
    Vector2 normal = collision.Normal;

    const std::shared_ptr<GameObject>& objectA = collision.ObjectA;
    const std::shared_ptr<GameObject>& objectB = collision.ObjectB;

    const std::shared_ptr<Collider>& colliderA = collision.ColliderA;
    const std::shared_ptr<Collider>& colliderB = collision.ColliderB;

    if (colliderA->IsSensor() || colliderB->IsSensor())
    {
        return;
    }

    if (colliderA->IsStatic() && colliderB->IsStatic())
    {
        return;
    }

    float offsetA = 0.0f;
    float offsetB = 0.0f;

    if (colliderA->IsStatic())
    {
        offsetB = 1.0f;
    }
    else if (colliderB->IsStatic())
    {
        offsetA = 1.0f;
    }
    else
    {
        Vector2 velocityA = objectA->Body->GetLinearVelocity();
        Vector2 velocityB = objectB->Body->GetLinearVelocity();

        float projectionA = fabs(velocityA.Dot(normal));
        float projectionB = fabs(velocityB.Dot(normal));
        float total = projectionA + projectionB;

        if (total > 0.0f)
        {
            offsetA = projectionA / total;
            offsetB = projectionB / total;
        }
        else
        {
            offsetA = 0.5f;
            offsetB = 0.5f;
        }
    }

    objectA->Transform->SetPosition(objectA->Transform->GetPosition() - normal * depth * offsetA);
    objectB->Transform->SetPosition(objectB->Transform->GetPosition() + normal * depth * offsetB);
}
