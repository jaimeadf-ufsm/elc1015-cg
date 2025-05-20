#include "GameObject.h"

GameObject::GameObject(std::reference_wrapper<Scene> scene) : m_Scene(scene), m_Priority(0)
{
    Transform = std::make_shared<::Transform>();
    Body = std::make_shared<::Body>();
}

void GameObject::Initialize()
{
}

void GameObject::Update(float deltaTime)
{
}

void GameObject::Draw(DrawingContext& context)
{
}

void GameObject::HandleEvent(const Event& event)
{
}

void GameObject::Collide(const Contact& contact)
{
}

void GameObject::AddTag(const std::string& tag)
{
    m_Tags.insert(tag);
}

void GameObject::RemoveTag(const std::string& tag)
{
    m_Tags.erase(tag);
}

bool GameObject::HasTag(const std::string& tag) const
{
    return m_Tags.count(tag) != 0;
}

void GameObject::SetPriority(int priority)
{
    m_Priority = priority;
}

int GameObject::GetPriority() const
{
    return m_Priority;
}

void GameObject::Destroy()
{
    m_Destroyed = true;
}

bool GameObject::HasBeenDestroyed() const
{
    return m_Destroyed;
}

BoundingBox GameObject::GetBoundingBox() const
{
    BoundingBox bb;

    for (const std::shared_ptr<Collider>& collider : Colliders)
    {
        for (const Vector2& localVertex : collider->GetPolygon()) 
        {
            Vector2 transformedVertex;
            Transform->Apply(localVertex, transformedVertex);

            bb.Encompass(transformedVertex);
        }
    }

    return bb;
}

Scene& GameObject::GetScene() const
{
    return m_Scene.get();
}
