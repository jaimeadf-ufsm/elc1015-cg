#include "Game.h"
#include "GameObject.h"

#include <algorithm>

Game::Game() : m_Mouse(), m_Keyboard(), m_PhysicsSystem(*this)
{
}

void Game::Update(float deltaTime)
{
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

void Game::Draw(DrawingContext& context)
{
    size_t n = m_Objects.size();

    for (size_t i = 0; i < n; i++)
    {
        m_Objects[i]->Draw(context);
    }
}

void Game::HandleEvent(const Event& event)
{
    switch (event.Type)
    {
        case EventType::MouseMove:
            m_Mouse.SetPosition(event.Mouse.Position);
            break;
        case EventType::MouseButtonPress:
            m_Mouse.SetButtonState(event.Mouse.Button, true);
            break;
        case EventType::MouseButtonRelease:
            m_Mouse.SetButtonState(event.Mouse.Button, false);
            break;
        case EventType::KeyPress:
            m_Keyboard.SetKeyState(event.Key.Key, true);
            break;
        case EventType::KeyRelease:
            m_Keyboard.SetKeyState(event.Key.Key, false);
            break;
    }
}

Mouse& Game::GetMouse()
{
    return m_Mouse;
}

Keyboard& Game::GetKeyboard()
{
    return m_Keyboard;
}

std::vector<std::shared_ptr<GameObject>>& Game::GetObjects()
{
    return m_Objects;
}

