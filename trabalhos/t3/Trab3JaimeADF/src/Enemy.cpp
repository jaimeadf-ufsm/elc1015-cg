#include "Enemy.h"
#include "Explosion.h"
#include "Game.h"

Enemy::Enemy(std::reference_wrapper<Game> game) : Entity(game)
{
    Health = std::make_shared<::Health>();
}

void Enemy::Initialize()
{
    Entity::Initialize();
    AddTag("Enemy");
}

void Enemy::Update(float deltaTime)
{
    Entity::Update(deltaTime);
}

void Enemy::Draw(DrawingContext& context)
{
    Entity::Draw(context);
}

void Enemy::Collide(const Contact& contact)
{
    Entity::Collide(contact);
}

void Enemy::Destroy()
{
    Entity::Destroy();
}