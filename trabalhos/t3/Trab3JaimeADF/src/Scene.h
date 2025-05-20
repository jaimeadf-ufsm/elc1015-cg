#pragma once

#include "Game.h"
#include "PhysicsSystem.h"
#include "AudioSystem.h"

class GameObject;

class Scene
{
public:
    Scene(std::reference_wrapper<Game> game);

    virtual void Start();
    virtual void Stop();

    virtual void Update(float deltaTime);
    virtual void Draw(DrawingContext& context);

    virtual void HandleEvent(const Event& event);

    template <typename T>
    std::shared_ptr<T> CreateObject();

    void DestroyObject(std::shared_ptr<GameObject> object);

    std::vector<std::shared_ptr<GameObject>>& GetObjects();

    template <typename T>
    std::shared_ptr<T> GetObject();

    Game& GetGame();

    Mouse& GetMouse();
    Keyboard& GetKeyboard();
    Viewport& GetViewport();

    GameState& GetState();

    AudioSystem& GetAudioSystem();
    PhysicsSystem& GetPhysicsSystem();

private:
    std::reference_wrapper<Game> m_Game;

    std::vector<std::shared_ptr<GameObject>> m_Objects;

    AudioSystem m_AudioSystem;
    PhysicsSystem m_PhysicsSystem;
};

template <typename T>
std::shared_ptr<T> Scene::CreateObject()
{
    std::shared_ptr<T> object = std::make_shared<T>(std::reference_wrapper<Scene>(*this));

    m_Objects.emplace_back(object);
    object->Initialize();

    return object;
}

template <typename T>
std::shared_ptr<T> Scene::GetObject()
{
    for (const std::shared_ptr<GameObject>& object : m_Objects)
    {
        std::shared_ptr<T> castedObject = std::dynamic_pointer_cast<T>(object);

        if (castedObject)
        {
            return castedObject;
        }
    }

    return nullptr;
}

