#pragma once

#include "GameObject.h"
#include "Track.h"
#include "TrackPlacer.h"

class PowerUpSpawner : public GameObject
{
public:
    PowerUpSpawner(std::reference_wrapper<Scene> scene);

    void Initialize() override;

    void Update(float deltaTime) override;
    void Draw(DrawingContext &context) override;

    void SetPlacer(std::shared_ptr<TrackPlacer> placer);
    std::shared_ptr<TrackPlacer> GetPlacer() const;

private:
    std::shared_ptr<TrackPlacer> m_Placer;

    void SpawnPowerUp();
    bool IsThereAnyPowerUp();
};