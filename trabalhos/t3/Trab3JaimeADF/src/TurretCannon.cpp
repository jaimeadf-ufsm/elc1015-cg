#include "TurretCannon.h"

static TesselatedGraphic s_PretesselatedGraphic = VectorGraphic({
    VectorFeature()
        .WithPaths("M14 4L21.15 4L22.6 2.54999C22.9667 2.18332 23.4333 2 24 2L30 2C30.5667 2 31.05 2.18332 31.45 2.54999C31.8167 2.94999 32 3.43333 32 4L32 10C32 10.5667 31.8167 11.0334 31.45 11.4C31.05 11.8 30.5667 12 30 12L24 12C23.4333 12 22.9667 11.8 22.6 11.4L21.15 10L14 10L14 8L22 8L24 10L29 10L30 10L30 4L29 4L24 4L22 6L14 6L14 4Z")
        .WithFill(ColorRGB(0x2E2B22)),
    VectorFeature()
        .WithPaths("M4 14L8 14L8 12L4 12L2 10L2 4L4 2L8 2L8 -3.49691e-07L4 -1.74846e-07C3.43333 -1.50076e-07 2.96666 0.183321 2.6 0.549988L0.599998 2.54999C0.199998 2.94999 -4.61884e-07 3.43333 -4.37114e-07 4L-1.74846e-07 10C-1.50076e-07 10.5667 0.199998 11.0334 0.599998 11.4L2.6 13.4C2.96667 13.8 3.43333 14 4 14Z")
        .WithFill(ColorRGB(0x8C2B22)),
    VectorFeature()
        .WithPaths("M14 8L14 6L22 6L24 4L29 4L29 10L24 10L22 8L14 8Z")
        .WithFill(ColorRGB(0x464133)),
    VectorFeature()
        .WithPaths("M4 12L8 12L8 8L8 6L8 2L4 2L2 4L2 10L4 12Z")
        .WithFill(ColorRGB(0xD04435)),
    VectorFeature()
        .WithPaths("M14 8L14 10C14 10.5667 13.8167 11.0334 13.45 11.4L11.45 13.4C11.05 13.8 10.5667 14 10 14L8 14L8 12L10 12L12 10L12 8L14 8ZM8 0L10 -8.74228e-08C10.5667 -1.12193e-07 11.05 0.183321 11.45 0.549988L13.45 2.54999C13.8167 2.94999 14 3.40002 14 3.90002L14 4L14 6L12 6L12 4L10 2L8 2L8 0Z")
        .WithFill(ColorRGB(0x8C2B22)),
    VectorFeature()
        .WithPaths("M8 8L8 6L12 6L14 6L14 8L12 8L8 8Z")
        .WithFill(ColorRGB(0xB63A2D)),
    VectorFeature()
        .WithPaths("M8 8L12 8L12 10L10 12L8 12L8 8ZM8 6L8 2L10 2L12 4L12 6L8 6Z")
        .WithFill(ColorRGB(0xD04435)),
    VectorFeature()
        .WithPaths("M29 4L30 4L30 10L29 10L29 4Z")
        .WithFill(ColorRGB(0x5B5442)),
}).Materialize();

TurretCannon::TurretCannon(std::reference_wrapper<Scene> scene) : Cannon::Cannon(scene)
{
}

void TurretCannon::Initialize()
{
    Cannon::Initialize();

    SetPriority(1);
    SetWidth(36.0f);
    SetFireRate(0.5f);
    SetIgnoredTags({ "Enemy" });

    m_Graphic = s_PretesselatedGraphic;
    m_Graphic.Translate(Vector2(-6.5f, -6.5f));
}

void TurretCannon::Update(float deltaTime)
{
    Cannon::Update(deltaTime);
}

void TurretCannon::Draw(DrawingContext& context)
{
    Cannon::Draw(context);
    m_Graphic.Draw(context, *Transform);
}
