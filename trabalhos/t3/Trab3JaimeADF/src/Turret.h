#pragma once

#include "Enemy.h"
#include "VectorGraphic.h"
#include "TurretCannon.h"

class Turret : public Enemy
{
public:
    Turret(std::reference_wrapper<Scene> scene);

    void Initialize() override;

    void Update(float deltaTime) override;
    void Draw(DrawingContext &context) override;

    void Destroy() override;

private:
    TesselatedGraphic m_Graphic;

    std::shared_ptr<TurretCannon> m_LeftCannon;
    std::shared_ptr<TurretCannon> m_RightCannon;
};