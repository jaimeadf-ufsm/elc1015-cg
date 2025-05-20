#include "MainScene.h"
#include "EditorScene.h"
#include "Player.h"
#include "Turret.h"
#include "PowerUp.h"
#include "Background.h"
#include "TrackBoundary.h"
#include "TrackPlacer.h"
#include "WaveSpawner.h"
#include "PowerUpSpawner.h"
#include "Score.h"
#include "GameOver.h"

MainScene::MainScene(std::reference_wrapper<Game> game) : Scene(game)
{
}

void MainScene::Start()
{
    Scene::Start();

    GameState& state = GetState();
    AudioSystem& audio = GetAudioSystem();

    Track track = *state.GetSelectedTrack();

    audio.LoadAudio("explosion", "Trab3JaimeADF/assets/sounds/explosion.wav");
    audio.LoadAudio("gunfire", "Trab3JaimeADF/assets/sounds/gunfire.wav");

    std::shared_ptr<Background> background = CreateObject<Background>();
    std::shared_ptr<TrackBoundary> trackBoundary = CreateObject<TrackBoundary>();
    std::shared_ptr<TrackPlacer> trackPlacer = CreateObject<TrackPlacer>();
    std::shared_ptr<WaveSpawner> waveSpawner = CreateObject<WaveSpawner>();
    std::shared_ptr<PowerUpSpawner> powerUpSpawner = CreateObject<PowerUpSpawner>();

    CreateObject<Score>();
    CreateObject<GameOver>();

    background->SetTrack(track);
    trackBoundary->SetTrack(track);
    trackPlacer->SetTrack(track);

    waveSpawner->SetPlacer(trackPlacer);
    powerUpSpawner->SetPlacer(trackPlacer);

    std::shared_ptr<Player> player = CreateObject<Player>();
    trackPlacer->PlaceOrDestroy(player);
}

void MainScene::Stop()
{
    Scene::Stop();
}

void MainScene::Update(float deltaTime)
{
    Scene::Update(deltaTime);
}

void MainScene::Draw(DrawingContext& context)
{
    Scene::Draw(context);
}

void MainScene::HandleEvent(const Event& event)
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