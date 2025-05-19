#include "TrackBoundary.h"

#include "Game.h"
#include "Constants.h"

#include "Tesselator.h"

TrackBoundary::TrackBoundary(std::reference_wrapper<Game> game) : GameObject::GameObject(game)
{

}

void TrackBoundary::Initialize()
{
    GameObject::Initialize();

    Path path;
    Mesh mesh;

    path.Arc(Vector2(1280.0f / 2.0f, 720.0f / 2.0f), 250.0f, 0.0f, 2 * PI);
    path.Close();

    Tesselator::Stroke(mesh, path, 16.0f);

    for (size_t i = 0; i + 3 <= mesh.Triangles.size(); i += 3)
    {
        std::shared_ptr<Collider> collider = std::make_shared<Collider>();
        collider->SetMode(ColliderMode::Static);
        collider->AddVertex(mesh.Vertices[mesh.Triangles[i]]);
        collider->AddVertex(mesh.Vertices[mesh.Triangles[i + 1]]);
        collider->AddVertex(mesh.Vertices[mesh.Triangles[i + 2]]);

        Colliders.emplace_back(collider);
    }

    Transform->SetPosition(Vector2());
}

void TrackBoundary::Update(float deltaTime)
{
    GameObject::Update(deltaTime);

    Game& game = GetGame();
    Keyboard& keyboard = game.GetKeyboard();

    // std::cout << deltaTime  * 1000.0f << std::endl;
}

void TrackBoundary::Draw(DrawingContext& context)
{
    GameObject::Draw(context);
}