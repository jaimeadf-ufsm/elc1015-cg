#include "Projectile.h"

#include "Game.h"
#include "Constants.h"
#include "Entity.h"
#include "Explosion.h"

static TesselatedGraphic s_PretesselatedGraphic = VectorGraphic({
    VectorFeature()
        .WithPaths("M9.99349 2.00641L5.99349 2.00641L5.99349 0.00640825L10.0435 0.00640843C12.1101 0.0730752 13.6101 0.623077 14.5435 1.65641C15.6101 2.62308 16.0935 4.02307 15.9935 5.85641L15.9935 5.95641L15.9935 6.15641C16.0935 7.98974 15.6101 9.38974 14.5435 10.3564C13.6101 11.3897 12.1101 11.9397 10.0435 12.0064L5.99349 12.0064L5.99349 10.0064L9.99349 10.0064C12.8602 9.90641 14.1935 8.57308 13.9935 6.00641C14.1935 3.43974 12.8602 2.10641 9.99349 2.00641Z")
        .WithFill(ColorRGB(0x1A7B44)),
    VectorFeature()
        .WithPaths("M5.99365 12.0064L1.99365 12.0064C1.42699 12.0064 0.94364 11.8231 0.54364 11.4564C0.176973 11.0564 -0.00634812 10.5731 -0.00634809 10.0064L-0.00634774 2.00641C-0.00634772 1.43974 0.176973 0.973074 0.54364 0.606407C0.94364 0.206407 1.42699 0.00640849 1.99365 0.00640852L5.99365 0.00640869L5.99365 2.00641L1.99365 2.00641L1.99365 10.0064L5.99365 10.0064L5.99365 12.0064Z")
        .WithFill(ColorRGB(0x6B8183)),
    VectorFeature()
        .WithPaths("M5.99365 2.00641L9.99365 2.00641L9.99365 10.0064L5.99365 10.0064L5.99365 2.00641Z")
        .WithFill(ColorRGB(0x2ECC71)),
    VectorFeature()
        .WithPaths("M5.99365 2.00641L5.99365 10.0064L1.99365 10.0064L1.99365 2.00641L5.99365 2.00641Z")
        .WithFill(ColorRGB(0x89A4A6)),
    VectorFeature()
        .WithPaths("M9.99362 2.00641C12.8603 2.10641 14.1936 3.43974 13.9936 6.00641C14.1936 8.57308 12.8603 9.90641 9.99362 10.0064L9.99362 2.00641Z")
        .WithFill(ColorRGB(0x27AF60)),
}).Materialize();

Projectile::Projectile(std::reference_wrapper<Game> game) :
    GameObject::GameObject(game),
    m_Damage(1.0f)
{

}

void Projectile::Initialize()
{
    GameObject::Initialize();

    Colliders = Collider::Box(Vector2(16.0f, 12.0f), ColliderMode::Sensor);

    AddTag("Projectile");

    m_Graphic = s_PretesselatedGraphic;
    m_Graphic.Translate(Vector2(-8.0f, -6.0f));
}

void Projectile::Update(float deltaTime)
{
    GameObject::Update(deltaTime);
    Body->SetLinearVelocity(Transform->GetHeading() * 600.0f);
}

void Projectile::Draw(DrawingContext& context)
{
    GameObject::Draw(context);
    m_Graphic.Draw(context, *Transform);
}

void Projectile::Collide(const Contact& contact)
{
    GameObject::Collide(contact);

    Game& game = GetGame();

    if (HasBeenDestroyed())
    {
        return;
    }

    if (contact.OtherCollider->IsSensor())
    {
        return;
    }

    for (const std::string& tag : m_IgnoredTags)
    {
        if (contact.OtherObject->HasTag(tag))
        {
            return;
        }
    }

    std::shared_ptr<Entity> entity = std::dynamic_pointer_cast<Entity>(contact.OtherObject);

    if (entity)
    {
        entity->Health->TakeDamage(m_Damage);
    }
    
    std::shared_ptr<Explosion> explosion = game.CreateObject<Explosion>();
    explosion->Transform->SetPosition(Transform->GetPosition());
    explosion->Transform->SetRotation(Transform->GetRotation());
    explosion->Transform->SetScale(Vector2(0.5f, 0.5f));

    Destroy();
}

void Projectile::SetDamage(float amount)
{
    m_Damage = amount;
}

float Projectile::GetDamage() const
{
    return m_Damage;
}

void Projectile::AddIgnoredTag(const std::string& tag)
{
    m_IgnoredTags.insert(tag);
}

void Projectile::RemoveIgnoredTag(const std::string& tag)
{
    m_IgnoredTags.erase(tag);
}

bool Projectile::HasIgnoredTag(const std::string& tag) const
{
    return m_IgnoredTags.count(tag) != 0;
}