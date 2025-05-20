#include "Enemy.h"
#include "Explosion.h"
#include "Scene.h"
#include "Score.h"

Enemy::Enemy(std::reference_wrapper<Scene> scene) : Entity(scene)
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

    otherEntity->Health->TakeDamage(contact.Duration * 25.0f);
}

void Enemy::Destroy()
{
    Entity::Destroy();

    Scene& scene = GetScene();

    if (Health->IsZero())
    {
        std::shared_ptr<Score> score = scene.GetObject<Score>();

        if (score)
        {
            score->IncrementScore(Health->GetMaxValue());
        }
    }
}