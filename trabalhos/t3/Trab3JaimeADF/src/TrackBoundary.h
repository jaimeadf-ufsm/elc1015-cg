#pragma once

#include "GameObject.h"
#include "Track.h"

class TrackBoundary : public GameObject
{
public:
    TrackBoundary(std::reference_wrapper<Scene> scene);

    void Initialize() override;

    void Update(float deltaTime) override;
    void Draw(DrawingContext &context) override;

    void Collide(const Contact& contact) override;

    void SetTrack(const Track& track);
    const Track& GetTrack() const;

private:
    Track m_Track;

    std::unordered_set<std::shared_ptr<GameObject>> m_AttackedEntities;
};