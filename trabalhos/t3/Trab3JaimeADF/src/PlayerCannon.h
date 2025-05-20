#pragma once

#include "Cannon.h"
#include "VectorGraphic.h"

class PlayerCannon : public Cannon
{
public:
    PlayerCannon(std::reference_wrapper<Scene> scene);

    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw(DrawingContext& content) override;

private:
    TesselatedGraphic m_Graphic;
};