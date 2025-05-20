#pragma once

#include "GameObject.h"
#include "Track.h"

class TrackPlacer : public GameObject
{
public:
    TrackPlacer(std::reference_wrapper<Scene> scene);

    void Initialize() override;

    void Update(float deltaTime) override;
    void Draw(DrawingContext &context) override;

    void PlaceOrDestroy(std::shared_ptr<GameObject> object, size_t retries = 5000);
    bool TryPlace(std::shared_ptr<GameObject> object, size_t retries = 5000);

    void SetTrack(const Track& track);
    const Track& GetTrack() const;

private:
    Track m_Track;

    Vector2 GetRandomPointOnTrack();
};