#include "EditorScene.h"
#include "TrackEditor.h"
#include "MainScene.h"
#include "ScoreScene.h"

EditorScene::EditorScene(std::reference_wrapper<Game> game) : Scene(game)
{
}

void EditorScene::Start()
{
    Scene::Start();
    CreateObject<TrackEditor>();
}

void EditorScene::Stop()
{
    Scene::Stop();
}

void EditorScene::Update(float deltaTime)
{
    Scene::Update(deltaTime);
}

void EditorScene::Draw(DrawingContext& context)
{
    Scene::Draw(context);
}

void EditorScene::HandleEvent(const Event& event)
{
    Scene::HandleEvent(event);

    Game& game = GetGame();
    GameState& state = game.GetState();

    switch (event.Type)
    {
        case EventType::KeyPress:
            switch (event.Key.Key)
            {
                case 'p':
                case 'P':
                    game.SwitchToScene<MainScene>();
                    break;
                case 's':
                case 'S':
                    game.SwitchToScene<ScoreScene>();
                    break;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    state.SelectTrack(event.Key.Key - '0');
                    break;
            }
            break;
        default:
            break;
    }
}