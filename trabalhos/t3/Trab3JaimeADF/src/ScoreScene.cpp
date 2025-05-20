#include "ScoreScene.h"
#include "EditorScene.h"
#include "Scoreboard.h"


ScoreScene::ScoreScene(std::reference_wrapper<Game> game) : Scene(game)
{
}

void ScoreScene::Start()
{
    Scene::Start();
    CreateObject<Scoreboard>();
}

void ScoreScene::Stop()
{
    Scene::Stop();
}

void ScoreScene::Update(float deltaTime)
{
    Scene::Update(deltaTime);
}

void ScoreScene::Draw(DrawingContext& context)
{
    Scene::Draw(context);
}

void ScoreScene::HandleEvent(const Event& event)
{
    Scene::HandleEvent(event);

    switch (event.Type)
    {
        case EventType::KeyPress:
            switch (event.Key.Key)
            {
                case KEY_ESC:
                    GetGame().SwitchToScene<EditorScene>();
                    break;
            }
            break;
        default:
            break;
    }
}