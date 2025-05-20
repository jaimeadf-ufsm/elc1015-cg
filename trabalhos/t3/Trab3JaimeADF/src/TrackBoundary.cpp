#include "TrackBoundary.h"

#include "Scene.h"
#include "Constants.h"
#include "Tesselator.h"
#include "Entity.h"

TrackBoundary::TrackBoundary(std::reference_wrapper<Scene> scene) : GameObject::GameObject(scene)
{
}

void TrackBoundary::Initialize()
{
    GameObject::Initialize();
    SetTrack(Track());
}

void TrackBoundary::Update(float deltaTime)
{
    GameObject::Update(deltaTime);
    m_AttackedEntities.clear();
}

void TrackBoundary::Draw(DrawingContext& context)
{
    GameObject::Draw(context);
}

void TrackBoundary::SetTrack(const Track& track)
{
    m_Track = track;

    const Mesh& outerBoundaryMesh = m_Track.ComputeOuterBoundaryMesh();
    const Mesh& innerBoundaryMesh = m_Track.ComputeInnerBoundaryMesh();

    Colliders = Collider::Meshes({ outerBoundaryMesh, innerBoundaryMesh }, ColliderMode::Static);
}

void TrackBoundary::Collide(const Contact& contact)
{
    GameObject::Collide(contact);

    std::shared_ptr<Entity> otherEntity = std::dynamic_pointer_cast<Entity>(contact.OtherObject);

    if (!otherEntity)
    {
        return;
    }

    if (!otherEntity->HasTag("Player"))
    {
        return;
    }

    if (m_AttackedEntities.count(otherEntity) != 0)
    {
        return;
    }

    m_AttackedEntities.insert(otherEntity);

    otherEntity->Health->TakeDamage(contact.Duration * 10.0f);
}

const Track& TrackBoundary::GetTrack() const
{
    return m_Track;
}