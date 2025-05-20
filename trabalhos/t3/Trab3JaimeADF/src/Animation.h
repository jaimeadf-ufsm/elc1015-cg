#pragma once

#include "GameObject.h"
#include "VectorGraphic.h"

class Animation : public GameObject
{
public:
    std::vector<TesselatedGraphic> Frames;

    Animation(std::reference_wrapper<Scene> scene);

    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw(DrawingContext& context) override;

    void SetDuration(float duration);
    float GetDuration() const;

private:
    float m_Duration;
    float m_Elapsed;
};