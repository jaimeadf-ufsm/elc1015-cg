#include "PlayerCannon.h"

static TesselatedGraphic s_PretesselatedGraphic = VectorGraphic({
    VectorFeature()
        .WithPaths("M17 8L20 10L28 10L28 2L20 2L18 4L2 4L2 8L18 8ZM-1.74846e-07 8L-3.49691e-07 4C-3.74461e-07 3.43333 0.199998 2.94999 0.599998 2.54999C0.966665 2.18332 1.43333 2 2 2L17.15 2L18.6 0.549987C18.9667 0.18332 19.4333 -8.49458e-07 20 -8.74228e-07L28 -1.22392e-06C28.5667 -1.24869e-06 29.05 0.18332 29.45 0.549987C29.8167 0.949986 30 1.43333 30 2L30 10C30 10.5667 29.8167 11.0333 29.45 11.4C29.05 11.8 28.5667 12 28 12L20 12C19.4333 12 18.9667 11.8 18.6 11.4L17.15 10L2 10C1.43333 10 0.966665 9.79999 0.599998 9.39999C0.199998 9.03333 -1.50076e-07 8.56667 -1.74846e-07 8Z")
        .WithFill(ColorRGB(0x17703D)),
    VectorFeature()
        .WithPaths("M17 8L18 4L20 2L28 2L28 10L20 10L18 8Z")
        .WithFill(ColorRGB(0x23A157)),
    VectorFeature()
        .WithPaths("M17 8L2 8L2 4L18 4L18 8Z")
        .WithFill(ColorRGB(0x20934f)),
}).Materialize();

PlayerCannon::PlayerCannon(std::reference_wrapper<Scene> scene) : Cannon::Cannon(scene)
{
}

void PlayerCannon::Initialize()
{
    Cannon::Initialize();

    SetWidth(30.0f);
    SetFireRate(4.0f);
    SetPriority(1);

    SetIgnoredTags({ "Player" });

    m_Graphic = s_PretesselatedGraphic;
    m_Graphic.Translate(Vector2(-4.0f, -6.0f));
}

void PlayerCannon::Update(float deltaTime)
{
    Cannon::Update(deltaTime);
}

void PlayerCannon::Draw(DrawingContext& context)
{
    Cannon::Draw(context);
    m_Graphic.Draw(context, *Transform);
}
