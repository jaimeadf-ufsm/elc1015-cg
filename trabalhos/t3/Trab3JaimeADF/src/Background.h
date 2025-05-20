#pragma once

#include "GameObject.h"
#include "Track.h"

class Background : public GameObject
{
public:
    Background(std::reference_wrapper<Scene> scene);

    void Initialize() override;

    void Update(float deltaTime) override;
    void Draw(DrawingContext &context) override;

    void SetTrack(const Track& track);
    const Track& GetTrack() const;

private:
    Track m_Track;
    TesselatedGraphic m_Graphic;
};