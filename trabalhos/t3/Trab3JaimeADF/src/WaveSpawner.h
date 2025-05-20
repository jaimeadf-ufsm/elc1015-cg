#pragma once

#include "GameObject.h"
#include "Track.h"
#include "TrackPlacer.h"

struct Wave
{
    int BoxCount;
    int BarrelCount;
    int TurretCount;
    int ShurikenCount;
};

class WaveSpawner : public GameObject
{
public:
    WaveSpawner(std::reference_wrapper<Scene> scene);

    void Initialize() override;

    void Update(float deltaTime) override;
    void Draw(DrawingContext &context) override;

    void SetPlacer(std::shared_ptr<TrackPlacer> placer);
    std::shared_ptr<TrackPlacer> GetPlacer() const;

private:
    std::shared_ptr<TrackPlacer> m_Placer;

    int m_Level;

    void SpawnLevel();
    bool HasEnemiesRemaing();
};