#include "Turret.h"

#include "Scene.h"
#include "Constants.h"

static const Vector2 s_LeftCannonOffset(-6.5, -7.5);
static const Vector2 s_RightCannonOffset(-6.5, 7.5);

static TesselatedGraphic s_PretesselatedGraphic = VectorGraphic({
    VectorFeature()
        .WithPaths("M2 6L2 4L4 2L22 2L22 6L2 6ZM44 6L44 12L28 12L28 6L44 6ZM44 36L44 42L28 42L28 36L44 36ZM4 46L2 44L2 42L22 42L22 46L4 46Z")
        .WithFill(ColorRGB(0xD04435)),
    VectorFeature()
        .WithPaths("M44 12L48 12L48 18L46 18L46 30L48 30L48 36L44 36L28 36L2 36L0 36L-1.74846e-07 32L4 32L10 32L10 30L10 18L10 16L4 16L-8.74228e-07 16L-1.04907e-06 12L2 12L28 12L44 12ZM14 18L20 18L20 16L14 16L14 18ZM14 32L20 32L20 30L14 30L14 32ZM26 32L38 32L40 30L40 18L38 16L26 16L24 18L24 30L26 32Z")
        .WithFill(ColorRGB(0x5F5D55)),
    VectorFeature()
        .WithPaths("M48 18L48 30L46 30L46 18L48 18ZM2 42L2 36L28 36L28 42L22 42L2 42ZM4 30L4 18L10 18L10 30L4 30ZM2 12L2 6L22 6L28 6L28 12L2 12ZM14 18L14 16L20 16L20 18L14 18ZM14 32L14 30L20 30L20 32L14 32ZM26 32L24 30L24 18L26 16L38 16L40 18L40 30L38 32L26 32Z")
        .WithFill(ColorRGB(0x56544D)),
    VectorFeature()
        .WithPaths("M4 32L4 30L10 30L10 32L4 32ZM4 18L4 16L10 16L10 18L4 18ZM28 28L28 20L36 20L36 28L28 28Z")
        .WithFill(ColorRGB(0x6B6960)),
    VectorFeature()
        .WithPaths("M4 2L6 -8.74228e-08L22 -7.86805e-07L22 2L4 2ZM40 4L42 4L44 6L28 6L28 4L40 4ZM44 42L42 44L40 44L28 44L28 42L44 42ZM22 46L22 48L6 48L4 46L22 46Z")
        .WithFill(ColorRGB(0xE74C3C)),
    VectorFeature()
        .WithPaths("M22 46L22 42L28 42L28 44L40 44L38 46L22 46ZM22 2L38 2L40 4L28 4L28 6L22 6L22 2Z")
        .WithFill(ColorRGB(0x403C2F)),
}).Materialize();

Turret::Turret(std::reference_wrapper<Scene> scene) : Enemy::Enemy(scene)
{
}

void Turret::Initialize()
{
    Enemy::Initialize();

    Scene& scene = GetScene();

    Health->SetMaxValue(10.0f);
    Health->SetValue(10.0f);

    Colliders = Collider::Box(Vector2(48.0f, 48.0f));

    m_Graphic = s_PretesselatedGraphic;
    m_Graphic.Translate(Vector2(-24.0f, -24.0f));

    m_LeftCannon = scene.CreateObject<TurretCannon>();
    m_RightCannon = scene.CreateObject<TurretCannon>();
}

void Turret::Update(float deltaTime)
{
    Enemy::Update(deltaTime);

    Scene& scene = GetScene();

    Vector2 position = Transform->GetPosition();
    Vector2 heading = Transform->GetHeading();
    Vector2 perpendicular = Vector2(-heading.Y, heading.X);

    m_LeftCannon->Transform->SetPosition(
        position + heading * s_LeftCannonOffset.X + perpendicular * s_LeftCannonOffset.Y
    );

    m_RightCannon->Transform->SetPosition(
        position + heading * s_RightCannonOffset.X + perpendicular * s_RightCannonOffset.Y
    );

    for (const std::shared_ptr<GameObject>& object : scene.GetObjects())
    {
        if (object->HasTag("Player"))
        {
            float angle = (object->Transform->GetPosition() - Transform->GetPosition()).Angle();

            m_LeftCannon->AimAt(angle);
            m_RightCannon->AimAt(angle);

            m_LeftCannon->Fire();
            m_RightCannon->Fire();

            break;
        }
    }
}

void Turret::Draw(DrawingContext& context)
{
    Enemy::Draw(context);
    m_Graphic.Draw(context, *Transform);
}

void Turret::Destroy()
{
    m_LeftCannon->Destroy();
    m_RightCannon->Destroy();
    Enemy::Destroy();
}