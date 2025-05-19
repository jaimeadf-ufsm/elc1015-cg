#include "Shuriken.h"

#include "Game.h"
#include "Constants.h"
#include "Tesselator.h"
#include "PathParser.h"


Shuriken::Shuriken(std::reference_wrapper<Game> game) : Entity::Entity(game)
{

}

void Shuriken::Initialize()
{
    Entity::Initialize();

    Health->SetMaxValue(10.0f);
    Health->SetValue(10.0f);

    std::vector<Mesh> bodyMeshes;
    std::vector<Path> bodyPaths = PathParser::ParseCommands("M42.8431 47.5782C44.2039 53.7251 50.0266 62 50.0266 62C34.4737 62 30.4041 48.2771 30.2717 47.5782C30.1393 46.8793 28.4785 37.06 28.4761 33.1748V33.1563C28.4785 29.2814 21.3858 27.6363 16.0339 25.9454C10.0504 24.055 0 24.9794 0 24.9794C7.77643 11.4588 21.4007 14.7463 22.3197 15.0167C23.2386 15.2871 32.3178 18.7271 35.6596 20.6667C39.0015 22.6063 43.9671 17.2628 48.1018 13.4558C52.7245 9.19935 56.9522 0 56.9522 0C64.7286 13.5206 55.2759 23.7896 54.3875 24.3845C53.4991 24.9794 46.1874 31.2209 42.8431 33.1563C39.4988 35.0917 41.626 42.0802 42.8431 47.5782Z");

    for (const Path& path : bodyPaths)
    {
        bodyMeshes.emplace_back();
        Mesh& bodyMesh = bodyMeshes.back();

        Tesselator::Fill(bodyMesh, path);

        bodyMesh.Translate(Vector2(-36.0f, -28.0f));
    }

    Colliders = Collider::Meshes(bodyMeshes);
    Transform->SetPosition(Vector2(1280 / 2, 720 / 2));

    m_Graphic = VectorGraphic({
      VectorFeature()
          .WithPaths(bodyPaths)
          .WithFill(ColorRGB(0x657A7B)),
    }).Materialize();

    m_Graphic.Translate(Vector2(-36.0f, -28.0f));
}

void Shuriken::Update(float deltaTime)
{
    Entity::Update(deltaTime);

    Game& game = GetGame();

    Body->SetAngularVelocity(PI);
    Body->SetLinearVelocity(Vector2());

    for (const std::shared_ptr<GameObject>& object : game.GetObjects())
    {
        if (object->HasTag("Player"))
        {
            Vector2 direction = object->Transform->GetPosition() - Transform->GetPosition();
            direction = direction.Normalized();

            Vector2 velocity = direction * 200.0f;

            Body->SetLinearVelocity(velocity);
        }
    }
    
    m_AttackedEntities.clear();
}

void Shuriken::Draw(DrawingContext& context)
{
    m_Graphic.Draw(context, *Transform);
    Entity::Draw(context);
}

void Shuriken::Collide(const Contact& contact)
{
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
