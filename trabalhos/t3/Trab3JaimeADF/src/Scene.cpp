#include <algorithm>

#include "Scene.h"
#include "GameObject.h"

Scene::Scene(std::reference_wrapper<Game> game) : m_Game(game), m_PhysicsSystem(*this)
{
}

void Scene::Start()
{
}

void Scene::Stop()
{
}

void Scene::Update(float deltaTime)
{
    std::sort(
        m_Objects.begin(),
        m_Objects.end(),
        [](const std::shared_ptr<GameObject>& a, const std::shared_ptr<GameObject>& b)
        {
            if (a->GetPriority() == b->GetPriority())
            {
                return a.get() < b.get();
            }

            return a->GetPriority() < b->GetPriority();
        }
    );

    size_t n = m_Objects.size();

    for (size_t i = 0; i < n; i++)
    {
        m_Objects[i]->Update(deltaTime);
    }

    auto it = std::remove_if(
        m_Objects.begin(),
        m_Objects.end(),
        [](std::shared_ptr<GameObject>& object) 
        {
            return object->HasBeenDestroyed();
        }
    );

    m_Objects.erase(it, m_Objects.end());

    m_PhysicsSystem.Simulate(deltaTime);
}

void Scene::Draw(DrawingContext& context)
{
    size_t n = m_Objects.size();

    for (size_t i = 0; i < n; i++)
    {
        m_Objects[i]->Draw(context);
    }
}

void Scene::HandleEvent(const Event& event)
{
    size_t n = m_Objects.size();

    for (size_t i = 0; i < n; i++)
    {
        m_Objects[i]->HandleEvent(event);
    }
}

Game& Scene::GetGame()
{
    return m_Game.get();
}

Mouse& Scene::GetMouse()
{
    return GetGame().GetMouse();
}

Keyboard& Scene::GetKeyboard()
{
    return GetGame().GetKeyboard();
}

Viewport& Scene::GetViewport()
{
    return GetGame().GetViewport();
}

GameState& Scene::GetState()
{
    return GetGame().GetState();
}

AudioSystem& Scene::GetAudioSystem()
{
    return m_AudioSystem;
}

PhysicsSystem& Scene::GetPhysicsSystem()
{
    return m_PhysicsSystem;
}

std::vector<std::shared_ptr<GameObject>>& Scene::GetObjects()
{
    return m_Objects;
}

