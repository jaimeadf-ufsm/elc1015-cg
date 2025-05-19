#include "Barrel.h"

#include "Game.h"
#include "Constants.h"

Barrel::Barrel(std::reference_wrapper<Game> game) : Enemy::Enemy(game)
{
}

void Barrel::Initialize()
{
    Enemy::Initialize();

    Health->SetMaxValue(10.0f);
    Health->SetValue(10.0f);

    Colliders = Collider::Circle(12.0f);

    Transform->SetPosition(Vector2(1280 / 2, 720 / 2));

    m_Graphic = VectorGraphic({
      VectorFeature()
          .WithPaths("M3.55 20.5C1.18334 18.1333 0 15.3 0 12C0 8.66667 1.18334 5.83333 3.55 3.5C5.85 1.16667 8.66667 0 12 0C15.2667 0 18.0833 1.16667 20.45 3.5H20.5C22.8333 5.83333 24 8.66667 24 12C24 15.3 22.8333 18.1333 20.5 20.5C18.1333 22.8333 15.3 24 12 24C8.66667 24 5.85 22.8167 3.55 20.45V20.5Z")
          .WithFill(ColorRGB(0x657A7B)),
      VectorFeature()
          .WithPaths("M19.05 4.89999C21.0167 6.86666 22 9.23333 22 12C22 14.7333 21.0167 17.0833 19.05 19.05C17.0833 21.0167 14.7333 22 12 22C9.23333 22 6.88333 21.0167 4.95 19.05C2.98333 17.0833 2 14.7333 2 12C2 9.23333 2.98333 6.86666 4.95 4.89999C6.88333 2.96666 9.23333 2 12 2C14.7333 2 17.0833 2.96666 19.05 4.89999Z")
          .WithFill(ColorRGB(0x89A4A6)),
      VectorFeature()
          .WithPaths("M17.65 6.35001C19.2167 7.91667 20 9.8 20 12C20 14.1667 19.2167 16.05 17.65 17.65C16.05 19.2167 14.1667 20 12 20C9.8 20 7.93334 19.2167 6.40001 17.65C4.80001 16.05 4 14.1667 4 12C4 9.8 4.80001 7.91667 6.40001 6.35001C7.93334 4.78334 9.8 4 12 4C14.2 4 16.0833 4.78334 17.65 6.35001Z")
          .WithFill(ColorRGB(0x799193)),
      VectorFeature()
          .WithPaths("M12 6C13.6333 6 15.05 6.6 16.25 7.8C17.4167 8.96667 18 10.3667 18 12C18 13.6333 17.4167 15.05 16.25 16.25C15.05 17.4167 13.6333 18 12 18C10.3667 18 8.98334 17.4167 7.85001 16.25C6.61667 15.05 6 13.6333 6 12C6 10.3667 6.6 8.96667 7.8 7.8L7.85001 7.75C8.98334 6.58333 10.3667 6 12 6Z")
          .WithFill(ColorRGB(0x89A4A6)),
      VectorFeature()
          .WithPaths("M15 9.5C15 9.9 14.85 10.25 14.55 10.55C14.25 10.85 13.9 11 13.5 11C13.0667 11 12.7167 10.85 12.45 10.55C12.15 10.25 12 9.9 12 9.5C12 9.1 12.15 8.75 12.45 8.45C12.7167 8.15 13.0667 8 13.5 8C13.9 8 14.25 8.15 14.55 8.45C14.85 8.75 15 9.1 15 9.5Z")
          .WithFill(ColorRGB(0x697E80)),
    }).Materialize();

    m_Graphic.Translate(Vector2(-14.0f, -14.0f));
}

void Barrel::Update(float deltaTime)
{
    Enemy::Update(deltaTime);
}

void Barrel::Draw(DrawingContext& context)
{
    Enemy::Draw(context);
    m_Graphic.Draw(context, *Transform);
}