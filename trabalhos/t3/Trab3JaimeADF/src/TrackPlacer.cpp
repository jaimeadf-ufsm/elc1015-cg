#include <stdlib.h>

#include "TrackPlacer.h"

#include "Scene.h"
#include "Constants.h"

#include "Tesselator.h"

TrackPlacer::TrackPlacer(std::reference_wrapper<Scene> scene) : GameObject::GameObject(scene)
{
}

void TrackPlacer::Initialize()
{
    GameObject::Initialize();
    SetTrack(Track());
}

void TrackPlacer::Update(float deltaTime)
{
    GameObject::Update(deltaTime);
}

void TrackPlacer::Draw(DrawingContext& context)
{
    GameObject::Draw(context);
}

void TrackPlacer::PlaceOrDestroy(std::shared_ptr<GameObject> object, size_t retries)
{
    if (!TryPlace(object, retries))
    {
        object->Destroy();
    }
}

bool TrackPlacer::TryPlace(std::shared_ptr<GameObject> object, size_t retries)
{
    Scene& scene = GetScene();
    PhysicsSystem& physicsSystem = scene.GetPhysicsSystem();

    std::vector<std::shared_ptr<GameObject>> objects;

    while (retries > 0)
    {
        Vector2 position = GetRandomPointOnTrack();

        object->Transform->SetPosition(position);
        BoundingBox bb = object->GetBoundingBox();

        physicsSystem.OverlapObjectsWithBox(objects, bb.GetMinPoint(), bb.GetMaxPoint());

        if (objects.empty())
        {
            return true;
        }

        if (objects.size() == 1 && objects[0] == object)
        {
            return true;
        }

        retries--;
    }

    return false;
}

void TrackPlacer::SetTrack(const Track& track)
{
    m_Track = track;
}

const Track& TrackPlacer::GetTrack() const
{
    return m_Track;
}

Vector2 TrackPlacer::GetRandomPointOnTrack()
{
    const std::vector<Vector2>& points = m_Track.ComputeCurve().GetPoints();

    size_t index = static_cast<size_t>(rand() % points.size());

    return points[index];
}