#include "Cannon.h"

#include "Scene.h"
#include "Constants.h"
#include "Projectile.h"

static const float s_ProjectileOffset = 10.0f;
static const float s_MuzzleFlashOffset = 10.0f;

Cannon::Cannon(std::reference_wrapper<Scene> scene) : GameObject::GameObject(scene)
{
}

void Cannon::Initialize()
{
    GameObject::Initialize();
    m_Width = 0.0f;
}

void Cannon::Update(float deltaTime)
{
    GameObject::Update(deltaTime);

    m_TimeSinceLastFire += deltaTime;

    if (m_MuzzleFlash)
    {
        m_MuzzleFlash->Transform->SetPosition(GetMuzzlePosition(s_MuzzleFlashOffset));
        m_MuzzleFlash->Transform->SetRotation(Transform->GetRotation());
    }
}

void Cannon::Draw(DrawingContext& context)
{
    GameObject::Draw(context);
}

void Cannon::Fire()
{
    Scene& scene = GetScene();

    if (m_TimeSinceLastFire <= (1.0f / m_FireRate))
    {
        return;
    }

    m_TimeSinceLastFire = 0.0f;

    std::shared_ptr<Projectile> projectile = scene.CreateObject<Projectile>();
    projectile->Transform->SetPosition(GetMuzzlePosition(s_ProjectileOffset));
    projectile->Transform->SetRotation(Transform->GetRotation());

    m_MuzzleFlash = scene.CreateObject<MuzzleFlash>();
    m_MuzzleFlash->Transform->SetPosition(GetMuzzlePosition(s_MuzzleFlashOffset));
    m_MuzzleFlash->Transform->SetRotation(Transform->GetRotation());

    for (const std::string& tag : m_IgnoredTags)
    {
        projectile->AddIgnoredTag(tag);
    }
}

void Cannon::AimAt(const Vector2& position)
{
    Transform->SetRotation((position - Transform->GetPosition()).Angle());
}

void Cannon::AimAt(float angle)
{
    Transform->SetRotation(angle);
}

void Cannon::SetWidth(float width)
{
    m_Width = width;
}

float Cannon::GetWidth() const
{
    return m_Width;
}

void Cannon::SetFireRate(float rate)
{
    m_FireRate = rate;
}

float Cannon::GetFireRate() const
{
    return m_FireRate;
}

void Cannon::SetIgnoredTags(const std::unordered_set<std::string>& tags)
{
    m_IgnoredTags = tags;
}

const std::unordered_set<std::string>& Cannon::GetIgnoredTags(const std::unordered_set<std::string>& tags) const
{
    return m_IgnoredTags;
}

Vector2 Cannon::GetMuzzlePosition(float offset) const
{
    return Transform->GetPosition() + Transform->GetHeading() * (m_Width + offset);
}