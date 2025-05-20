#include "WaveSpawner.h"

#include "Scene.h"
#include "Constants.h"
#include "Tesselator.h"
#include "Entity.h"

#include "Box.h"
#include "Barrel.h"
#include "Turret.h"
#include "Shuriken.h"
#include "Message.h"

static Wave GenerateLevelWave(int level)
{
    Wave wave;

    switch (level)
    {
        case 1:
            wave = Wave {
                .BoxCount = 2,
                .BarrelCount = 2,
                .TurretCount = 0,
                .ShurikenCount = 0
            };
            break;
        case 2:
            wave = Wave {
                .BoxCount = 2,
                .BarrelCount = 2,
                .TurretCount = 1,
                .ShurikenCount = 1
            };
            break;
        default:
            wave = Wave {
                .BoxCount = 1,
                .BarrelCount = 1,
                .TurretCount = 2,
                .ShurikenCount = 1
            };
            break;
    }

    return wave;
}

WaveSpawner::WaveSpawner(std::reference_wrapper<Scene> scene) : GameObject::GameObject(scene)
{
}

void WaveSpawner::Initialize()
{
    GameObject::Initialize();
    m_Level = 1;
}

void WaveSpawner::Update(float deltaTime)
{
    GameObject::Update(deltaTime);

    if (!m_Placer)
    {
        return;
    }

    if (!HasEnemiesRemaing())
    {
        SpawnLevel();
    }
}

void WaveSpawner::Draw(DrawingContext& context)
{
    GameObject::Draw(context);
}

void WaveSpawner::SetPlacer(std::shared_ptr<TrackPlacer> placer)
{
    m_Placer = placer;
}

std::shared_ptr<TrackPlacer> WaveSpawner::GetPlacer() const
{
    return m_Placer;
}

void WaveSpawner::SpawnLevel()
{
    Scene& scene = GetScene();

    if (!m_Placer)
    {
        return;
    }

    Wave wave = GenerateLevelWave(m_Level);

    for (int i = 0; i < wave.BoxCount; i++)
    {
        std::shared_ptr<GameObject> box = scene.CreateObject<Box>();
        m_Placer->PlaceOrDestroy(box);
    }

    for (int i = 0; i < wave.BarrelCount; i++)
    {
        std::shared_ptr<GameObject> barrel = scene.CreateObject<Barrel>();
        m_Placer->PlaceOrDestroy(barrel);
    }

    for (int i = 0; i < wave.TurretCount; i++)
    {
        std::shared_ptr<GameObject> turret = GetScene().CreateObject<Turret>();
        m_Placer->PlaceOrDestroy(turret);
    }

    for (int i = 0; i < wave.ShurikenCount; i++)
    {
        std::shared_ptr<GameObject> shuriken = GetScene().CreateObject<Shuriken>();
        m_Placer->PlaceOrDestroy(shuriken);
    }
    
    std::shared_ptr<Message> message = scene.CreateObject<Message>();
    message->SetContent("Level " + std::to_string(m_Level));
    
    m_Level++;
}

bool WaveSpawner::HasEnemiesRemaing()
{
    Scene& scene = GetScene();
    std::vector<std::shared_ptr<GameObject>>& objects = scene.GetObjects();

    for (const std::shared_ptr<GameObject>& object : objects)
    {
        if (object->HasTag("Enemy"))
        {
            return true;
        }
    }

    return false;
}

