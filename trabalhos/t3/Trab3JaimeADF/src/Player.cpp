#include "Player.h"

#include "Scene.h"
#include "Constants.h"

#include <cmath>

static const TesselatedGraphic s_PretesselatedGraphic = VectorGraphic({
    VectorFeature()
        .WithPaths("M2 12V30H38V12H2Z")
        .WithFill(ColorRGB(0x2ECC71)),
    VectorFeature()
        .WithPaths("M38 7.15002L39.4 8.59998C39.8 8.96664 40 9.43333 40 10L40 32C40 32.5667 39.8 33.05 39.4 33.45L38 34.85L36 34L38 32L38 30L38 28L38 24L38 18L38 14L38 12L38 10L36 8L38 7.15002ZM2 34.5L0.199996 30.9L-9.98804e-07 30L-2.11999e-07 12L0.199997 11.1L2 7.5L4 8L2 12L2 16L2 26L2 30L4 34L2 34.5Z")
        .WithFill(ColorRGB(0x1A7B44)),
    VectorFeature()
        .WithPaths("M36 8L38 10L38 12L2.00005 12L4.00005 8L8.00005 8L32 8L36 8ZM38 30L38 32L36 34L32 34L8.00005 34L4.00005 34L2.00005 30L38 30Z")
        .WithFill(ColorRGB(0x27AF60)),
    VectorFeature()
        .WithPaths("M38 34.85L38 38C38 38.5667 37.8 39.05 37.4 39.45L35.4 41.45C35.0333 41.8167 34.5667 42 34 42L5.9 42C5.4 42 4.95 41.8167 4.55 41.45L2.55 39.45C2.18333 39.05 2 38.5667 2 38L2 34.5L4 34L4 38L6 40L8 40L8 38L12 38L12 40L18 40L18 38L22 38L22 40L28 40L28 38L32 38L32 40L34 40L36 38L36 34L38 34.85ZM2 7.5L2 4C2 3.43333 2.18334 2.96664 2.55 2.59997L4.55 0.599974C4.95 0.199974 5.4 -1.42499e-06 5.9 -1.40314e-06L34 -1.74846e-07C34.5667 -1.50076e-07 35.0333 0.199975 35.4 0.599975L37.4 2.59998C37.8 2.96664 38 3.43333 38 4L38 7.15002L36 8L36 4L34 2L32 2L32 4L28 4L28 2L22 2L22 4L18 4L18 2L12 2L12 4L8 4L8 2L6 2L4 4L4 8L2 7.5Z")
        .WithFill(ColorRGB(0x2E2B22)),
    VectorFeature()
        .WithPaths("M32 4L32 2L34 2L36 4L36 8L32 8L32 4ZM36 34L36 38L34 40L32 40L32 38L32 34L36 34ZM8 38L8 40L6 40L4 38L4 34L8 34L8 38ZM4 8L4 4L6 2L8 2L8 4L8 8L4 8Z")
        .WithFill(ColorRGB(0x464133)),
    VectorFeature()
        .WithPaths("M32 4L32 8L8 8L8 4L12 4L12 2L18 2L18 4L22 4L22 2L28 2L28 4L32 4ZM32 38L28 38L28 40L22 40L22 38L18 38L18 40L12 40L12 38L8 38L8 34L32 34L32 38Z")
        .WithFill(ColorRGB(0x403C2F)),
    VectorFeature()
        .WithPaths("M38 14L38 18L28 18L28 14L38 14ZM38 24L38 28L28 28L28 24L38 24ZM2 26L2 16L6 16L6 26L2 26Z")
        .WithFill(ColorRGB(0x27AF60)),
    VectorFeature()
        .WithPaths("M26 18L24 16L16 16L14 18L14 24L16 26L24 26L26 24L26 18Z")
        .WithFill(ColorRGB(0x27AF60)),
}).Materialize();

Player::Player(std::reference_wrapper<Scene> scene) : Entity::Entity(scene)
{
}

void Player::Initialize()
{
    Entity::Initialize();

    Scene& scene = GetScene();

    Colliders = Collider::Box(Vector2(40.0f, 42.0f));

    Health->SetMaxValue(50.0f);
    Health->SetValue(50.0f);

    AddTag("Player");
    
    m_Graphic = s_PretesselatedGraphic;
    m_Graphic.Translate(Vector2(-20.0f, -21.0f));

    m_InvincibleTimeRemaining = 0.0f;
    m_RapidFireTimeRemaining = 0.0f;

    m_Cannon = scene.CreateObject<PlayerCannon>();
}

void Player::Update(float deltaTime)
{
    Entity::Update(deltaTime);

    Scene& scene = GetScene();
    Mouse& mouse = scene.GetMouse();
    Keyboard& keyboard = scene.GetKeyboard();

    m_RapidFireTimeRemaining -= deltaTime;
    m_InvincibleTimeRemaining -= deltaTime;

    m_Cannon->Transform->SetPosition(Transform->GetPosition());
    m_Cannon->AimAt(mouse.GetPosition());

    if (m_InvincibleTimeRemaining > 0.0f)
    {
        Health->SetInvincible(true);
    }
    else
    {
        Health->SetInvincible(false);
    }

    if (m_RapidFireTimeRemaining > 0.0f)
    {
        m_Cannon->SetFireRate(8.0f);
    }
    else
    {
        m_Cannon->SetFireRate(4.0f);
    }

    Vector2 linearVelocity = Transform->GetHeading() * 150.0f;
    float angularVelocity = 0.0f;

    if (keyboard.IsKeyPressed(KEY_A))
    {
        angularVelocity -= PI;
    }

    if (keyboard.IsKeyPressed(KEY_D))
    {
        angularVelocity += PI;
    }

    if (mouse.IsButtonPressed(MOUSE_BUTTON_LEFT))
    {
        m_Cannon->Fire();
    }

    Body->SetLinearVelocity(linearVelocity);
    Body->SetAngularVelocity(angularVelocity);
}

void Player::Draw(DrawingContext& context)
{
    Entity::Draw(context);
    m_Graphic.Draw(context, *Transform);
}

void Player::Destroy()
{
    m_Cannon->Destroy();
    Entity::Destroy();
}

void Player::ApplyEffect(Effect effect)
{
    switch (effect)
    {
    case Effect::Heal:
        Health->SetValue(Health->GetMaxValue());
        break;
    case Effect::Shield:
        m_InvincibleTimeRemaining = 5.0f;
        break;
    case Effect::RapidFire:
        m_RapidFireTimeRemaining = 5.0f;
        break;
    }
}