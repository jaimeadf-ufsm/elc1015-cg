#include "Entity.h"
#include "Explosion.h"
#include "Scene.h"

Entity::Entity(std::reference_wrapper<Scene> scene) : GameObject(scene)
{
    Health = std::make_shared<::Health>();
}

void Entity::Initialize()
{

}

void Entity::Update(float deltaTime)
{
    GameObject::Update(deltaTime);

    if (Health->IsZero())
    {
        Destroy();
    }
}

void Entity::Draw(DrawingContext& context)
{
    BoundingBox bb = GetBoundingBox();

    Vector2 minPoint = bb.GetMinPoint();
    Vector2 maxPoint = bb.GetMaxPoint();

    float objectWidth = maxPoint.X - minPoint.X;
    float healthBarWidth = 48.0f;
    float healthBarHeight = 8.0f;

    float healthValueWidth = healthBarWidth * Health->ComputePercetange();

    Vector2 healthPosition = Vector2(
        minPoint.X + (objectWidth - healthBarWidth) / 2.0f,
        maxPoint.Y + 8.0f
    );

    context.Color(Health->IsInvincible() ? 0x585858 : 0x0C511F);
    context.FillRectangle(healthPosition, Vector2(healthBarWidth, healthBarHeight));

    context.Color(Health->IsInvincible() ? 0xD7D7D7 : 0x0ECA43);
    context.FillRectangle(healthPosition, Vector2(healthValueWidth, healthBarHeight));

    GameObject::Draw(context);
}

void Entity::Collide(const Contact& contact)
{
    GameObject::Collide(contact);
}

void Entity::Destroy()
{
    GameObject::Destroy();
}