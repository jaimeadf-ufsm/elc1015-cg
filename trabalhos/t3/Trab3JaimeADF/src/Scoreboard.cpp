#include <cstdlib>
#include <algorithm>

#include "Scoreboard.h"

#include "Scene.h"
#include "Constants.h"

#include "Tesselator.h"
#include "TextRuler.h"
#include "Player.h"
#include "Score.h"

Scoreboard::Scoreboard(std::reference_wrapper<Scene> scene) : GameObject::GameObject(scene)
{
}

void Scoreboard::Initialize()
{
    GameObject::Initialize();
}

void Scoreboard::Update(float deltaTime)
{
    GameObject::Update(deltaTime);
}

void Scoreboard::Draw(DrawingContext& context)
{
    GameObject::Draw(context);

    Scene& scene = GetScene();
    GameState& state = scene.GetState();
    Viewport& viewport = scene.GetViewport();

    std::vector<std::shared_ptr<ScoreRecord>> scores = state.Scores;

    std::sort(
        scores.begin(), 
        scores.end(), 
        [](const std::shared_ptr<ScoreRecord>& a, const std::shared_ptr<ScoreRecord>& b)
        {
            return a->Score > b->Score;
        }
    );

    context.Color(0xD06C3B);
    context.FillRectangle(Vector2(0.0f, 0.0f), Vector2(viewport.GetWidth(), viewport.GetHeight()));

    context.Color(0xFFFFFF);
    context.Text(Vector2(32.0f, 32.0f), "Scoreboard");

    for (size_t i = 0; i < scores.size(); ++i)
    {
        std::string text = std::to_string(scores[i]->Score);
        context.Text(Vector2(32.0f, 64.0f + i * 32.0f), text);
    }
}
