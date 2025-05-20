#include "Shuriken.h"

#include "Scene.h"
#include "Constants.h"
#include "Tesselator.h"
#include "PathParser.h"

static const TesselatedGraphic s_PretesselatedGraphic = VectorGraphic({
    VectorFeature()
        .WithPaths("M24 0L29.6129 17.2746H47.7765L33.0818 27.9508L38.6947 45.2254L24 34.5491L9.30541 45.2254L14.9183 27.9508L0.223633 17.2746H18.3872L24 0Z")
        .WithFill(ColorRGB(0xD04436)),
    VectorFeature()
        .WithPaths("M19.1133 18.2744H3.30078L16.0938 27.5693L11.207 42.6064L24 33.3135L36.792 42.6064L31.9062 27.5693L44.6992 18.2744H28.8867L24 3.23535L19.1133 18.2744Z")
        .WithFill(ColorRGB(0xE74C3C)),
}).Materialize();

Shuriken::Shuriken(std::reference_wrapper<Scene> scene) : Enemy::Enemy(scene)
{

}

void Shuriken::Initialize()
{
    Enemy::Initialize();

    Health->SetMaxValue(10.0f);
    Health->SetValue(10.0f);

    std::vector<Mesh> bodyMeshes;
    std::vector<Path> bodyPaths = PathParser::ParseCommands("M24 0L29.6129 17.2746H47.7765L33.0818 27.9508L38.6947 45.2254L24 34.5491L9.30541 45.2254L14.9183 27.9508L0.223633 17.2746H18.3872L24 0Z");

    for (const Path& path : bodyPaths)
    {
        bodyMeshes.emplace_back();
        Mesh& bodyMesh = bodyMeshes.back();

        Tesselator::Fill(bodyMesh, path);

        bodyMesh.Translate(Vector2(-23.28f, -24.5f));
    }

    Colliders = Collider::Meshes(bodyMeshes);

    m_Graphic = s_PretesselatedGraphic;
    m_Graphic.Translate(Vector2(-23.28f, -24.5f));
}

void Shuriken::Update(float deltaTime)
{
    Enemy::Update(deltaTime);

    Scene& scene = GetScene();

    Body->SetAngularVelocity(PI / 4.0f);
    Body->SetLinearVelocity(Vector2());

    for (const std::shared_ptr<GameObject>& object : scene.GetObjects())
    {
        if (object->HasTag("Player"))
        {
            Vector2 direction = object->Transform->GetPosition() - Transform->GetPosition();
            direction = direction.Normalized();

            Vector2 velocity = direction * 50.0f;

            Body->SetLinearVelocity(velocity);
        }
    }
}

void Shuriken::Draw(DrawingContext& context)
{
    m_Graphic.Draw(context, *Transform);
    Enemy::Draw(context);
}
