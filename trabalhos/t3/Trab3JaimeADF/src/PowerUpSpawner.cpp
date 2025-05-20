#include "PowerUpSpawner.h"

#include "Scene.h"
#include "Constants.h"
#include "Tesselator.h"
#include "Entity.h"
#include "PowerUp.h"

PowerUpSpawner::PowerUpSpawner(std::reference_wrapper<Scene> scene) : GameObject::GameObject(scene)
{
}

void PowerUpSpawner::Initialize()
{
    GameObject::Initialize();
}

void PowerUpSpawner::Update(float deltaTime)
{
    GameObject::Update(deltaTime);

    if (!m_Placer)
    {
        return;
    }

    if (!IsThereAnyPowerUp())
    {
        SpawnPowerUp();
    }
}

void PowerUpSpawner::Draw(DrawingContext& context)
{
    GameObject::Draw(context);
}

void PowerUpSpawner::SetPlacer(std::shared_ptr<TrackPlacer> placer)
{
    m_Placer = placer;
}

std::shared_ptr<TrackPlacer> PowerUpSpawner::GetPlacer() const
{
    return m_Placer;
}

void PowerUpSpawner::SpawnPowerUp()
{
    Scene& scene = GetScene();

    if (!m_Placer)
    {
        return;
    }

    std::shared_ptr<PowerUp> powerUp = scene.CreateObject<PowerUp>();
    m_Placer->PlaceOrDestroy(powerUp);
}

bool PowerUpSpawner::IsThereAnyPowerUp()
{
    Scene& scene = GetScene();

    for (const std::shared_ptr<GameObject>& object : scene.GetObjects())
    {
        if (object->HasTag("PowerUp"))
        {
            return true;
        }
    }

    return false;
}

