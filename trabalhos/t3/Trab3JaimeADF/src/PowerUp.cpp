#include <cstdlib>
#include <cmath>
#include <ctime>

#include "PowerUp.h"
#include "Player.h"

static const TesselatedGraphic s_HealPretesselatedGraphic = VectorGraphic({
    VectorFeature()
        .WithPaths("M24 12C24 18.5 18.6274 24 12 24C5.37258 24 0 18.6274 0 12C0 5.37258 5.37258 0 12 0C18.6274 0 24 5.37258 24 12Z")
        .WithFill(ColorRGB(0x91E451)),
    VectorFeature()
        .WithPaths("M11.1665 16.5V7.16667C11.1665 6.79848 11.465 6.5 11.8332 6.5C12.2014 6.5 12.4998 6.79848 12.4998 7.16667V16.5C12.4998 16.8682 12.2014 17.1667 11.8332 17.1667C11.465 17.1667 11.1665 16.8682 11.1665 16.5Z")
        .WithFill(ColorRGB(0x4F8426)),
    VectorFeature()
        .WithPaths("M16.5 11.1667C16.8682 11.1667 17.1667 11.4651 17.1667 11.8333C17.1667 12.2015 16.8682 12.5 16.5 12.5H7.16667C6.79848 12.5 6.5 12.2015 6.5 11.8333C6.5 11.4651 6.79848 11.1667 7.16667 11.1667H16.5Z")
        .WithFill(ColorRGB(0x4F8426)),
}).Materialize();

static const TesselatedGraphic s_ShieldPretesselatedGraphic = VectorGraphic({
    VectorFeature()
        .WithPaths("M24 12C24 18.5 18.6274 24 12 24C5.37258 24 0 18.6274 0 12C0 5.37258 5.37258 0 12 0C18.6274 0 24 5.37258 24 12Z")
        .WithFill(ColorRGB(0x376BDB)),
    VectorFeature()
        .WithPaths("M11.8333 18.8333C11.8333 18.8333 17.1667 16.1667 17.1667 12.1667V7.5L11.8333 5.5L6.5 7.5V12.1667C6.5 16.1667 11.8333 18.8333 11.8333 18.8333Z")
        .WithFill(ColorRGB(0x2A4D97)),
}).Materialize();

static const TesselatedGraphic s_RapidFirePretesselatedGraphic = VectorGraphic({
    VectorFeature()
        .WithPaths("M24 12C24 18.5 18.6274 24 12 24C5.37258 24 0 18.6274 0 12C0 5.37258 5.37258 0 12 0C18.6274 0 24 5.37258 24 12Z")
        .WithFill(ColorRGB(0xE45D51)),
    VectorFeature()
        .WithPaths("M19.3333 12C19.3333 16.0501 16.05 19.3333 12 19.3333C7.94987 19.3333 4.66663 16.0501 4.66663 12C4.66663 7.94992 7.94987 4.66667 12 4.66667C16.05 4.66667 19.3333 7.94992 19.3333 12Z")
        .WithFill(ColorRGB(0x94423A)),
    VectorFeature()
        .WithPaths("M18 12C18 15.5 15.3137 18 12 18C8.68629 18 6 15.3137 6 12C6 8.68629 8.68629 6 12 6C15.3137 6 18 8.68629 18 12Z")
        .WithFill(ColorRGB(0xE45D51)),
    VectorFeature()
        .WithPaths("M18.6667 11.3333C19.0349 11.3333 19.3334 11.6318 19.3334 12C19.3334 12.3682 19.0349 12.6667 18.6667 12.6667H16C15.6319 12.6667 15.3334 12.3682 15.3334 12C15.3334 11.6318 15.6319 11.3333 16 11.3333H18.6667Z")
        .WithFill(ColorRGB(0x94423A)),
    VectorFeature()
        .WithPaths("M7.99996 11.3333C8.36815 11.3333 8.66663 11.6318 8.66663 12C8.66663 12.3682 8.36815 12.6667 7.99996 12.6667H5.33329C4.9651 12.6667 4.66663 12.3682 4.66663 12C4.66663 11.6318 4.9651 11.3333 5.33329 11.3333H7.99996Z")
        .WithFill(ColorRGB(0x94423A)),
    VectorFeature()
        .WithPaths("M11.3334 8.00001V5.33334C11.3334 4.96515 11.6319 4.66667 12 4.66667C12.3682 4.66667 12.6667 4.96515 12.6667 5.33334V8.00001C12.6667 8.3682 12.3682 8.66667 12 8.66667C11.6319 8.66667 11.3334 8.3682 11.3334 8.00001Z")
        .WithFill(ColorRGB(0x94423A)),
    VectorFeature()
        .WithPaths("M11.3334 18.6667V16C11.3334 15.6318 11.6319 15.3333 12 15.3333C12.3682 15.3333 12.6667 15.6318 12.6667 16V18.6667C12.6667 19.0349 12.3682 19.3333 12 19.3333C11.6319 19.3333 11.3334 19.0349 11.3334 18.6667Z")
        .WithFill(ColorRGB(0x94423A)),
}).Materialize();

PowerUp::PowerUp(std::reference_wrapper<Scene> scene) : GameObject(scene)
{
}

void PowerUp::Initialize()
{
    AddTag("PowerUp");

    Colliders = Collider::Box(Vector2(24.0f, 24.0f), ColliderMode::Sensor);

    m_Effect = static_cast<Effect>(std::rand() % 3);

    switch (m_Effect)
    {
    case Heal:
        m_Graphic = s_HealPretesselatedGraphic;
        break;
    case Shield:
        m_Graphic = s_ShieldPretesselatedGraphic;
        break;
    case RapidFire:
        m_Graphic = s_RapidFirePretesselatedGraphic;
        break;
    }

    m_Graphic.Translate(Vector2(-12.0f, -12.0f));
}

void PowerUp::Update(float deltaTime)
{
    GameObject::Update(deltaTime);
    m_Animation += deltaTime;
}

void PowerUp::Draw(DrawingContext& context)
{
    GameObject::Draw(context);
    
    ::Transform transform;
    transform.SetPosition(Transform->GetPosition());
    transform.SetScale(Transform->GetScale() * (1.25f + std::sin(2 * m_Animation) * 0.25f));

    m_Graphic.Draw(context, transform);
}

void PowerUp::Collide(const Contact& contact)
{
    if (HasBeenDestroyed())
    {
        return;
    }

    std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(contact.OtherObject);

    if (player)
    {
        player->ApplyEffect(m_Effect);
        Destroy();
    }
}