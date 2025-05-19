#include "Box.h"

#include "Game.h"
#include "Constants.h"

static const TesselatedGraphic s_PretesselatedGraphic = VectorGraphic({
    VectorFeature()
        .WithPaths("M2 0H26C26.5667 0 27.05 0.200006 27.45 0.600006C27.8167 0.966673 28 1.43333 28 2V26C28 26.5667 27.8167 27.05 27.45 27.45C27.05 27.8167 26.5667 28 26 28H2C1.43333 28 0.966673 27.8167 0.600006 27.45C0.200006 27.05 0 26.5667 0 26V2C0 1.43333 0.200006 0.966673 0.600006 0.600006C0.966673 0.200006 1.43333 0 2 0ZM2 2V26H26V2H2Z")
        .WithFill(ColorRGB(0x875C30)),
    VectorFeature()
        .WithPaths("M18 22L6 10L10 6H6V10V22H18H22V18L18 22ZM22 18V6H10L22 18ZM2 2H26V26H2V2Z")
        .WithFill(ColorRGB(0xBB8044)),
    VectorFeature()
        .WithPaths("M6 10L18 22H6V10ZM22 18L10 6H22V18Z")
        .WithFill(ColorRGB(0x85592E)),
    VectorFeature()
        .WithPaths("M6 10V6H10L6 10ZM18 22L22 18V22H18Z")
        .WithFill(ColorRGB(0xAF783F)),
}).Materialize();

Box::Box(std::reference_wrapper<Game> game) : Enemy::Enemy(game)
{
}

void Box::Initialize()
{
    Enemy::Initialize();

    Health->SetMaxValue(10.0f);
    Health->SetValue(10.0f);

    Colliders = Collider::Box(Vector2(28.0f, 28.0f));

    Transform->SetPosition(Vector2(1280 / 2, 720 / 2));

    m_Graphic = s_PretesselatedGraphic;
    m_Graphic.Translate(Vector2(-14.0f, -14.0f));
}

void Box::Update(float deltaTime)
{
    Enemy::Update(deltaTime);
}

void Box::Draw(DrawingContext& context)
{
    Enemy::Draw(context);
    m_Graphic.Draw(context, *Transform);
}