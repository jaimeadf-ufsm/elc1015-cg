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
    m_AttackedEntities.clear();
}

void Enemy::Draw(DrawingContext& context)
{
    Entity::Draw(context);
}

void Enemy::Collide(const Contact& contact)
{
    Entity::Collide(contact);

    std::shared_ptr<Entity> otherEntity = std::dynamic_pointer_cast<Entity>(contact.OtherObject);

    if (!otherEntity)
    {
        return;
    }

    if (!otherEntity->HasTag("Player"))
    {
        return;
    }

    if (m_AttackedEntities.count(otherEntity) != 0)
    {
        return;
    }

    m_AttackedEntities.insert(otherEntity);

    otherEntity->Health->TakeDamage(contact.Duration * 100.0f);
}

void Enemy::Destroy()
{
    Entity::Destroy();
}