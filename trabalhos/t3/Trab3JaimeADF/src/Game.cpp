#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

Game::Game() : m_Mouse(), m_Keyboard(), m_Viewport()
{
    m_Viewport.SetWidth(1280);
    m_Viewport.SetHeight(720);
}

void Game::Update(float deltaTime)
{
    if (m_NextScene)
    {
        if (m_CurrentScene)
        {
            m_CurrentScene->Stop();
        }

        m_CurrentScene = m_NextScene;
        m_NextScene = nullptr;

        m_CurrentScene->Start();
    }

    if (m_CurrentScene)
    {
        const float maxTimeStep = 1.0f / 60.0f;

        float remainingTime = deltaTime;

        while (remainingTime > 0.0001f) {
            float fragmentTime = std::min(remainingTime, maxTimeStep);
            m_CurrentScene->Update(fragmentTime);

            remainingTime -= fragmentTime;
        }
    }
}

void Game::Draw(DrawingContext& context)
{
    if (m_CurrentScene)
    {
        m_CurrentScene->Draw(context);
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

    if (m_CurrentScene)
    {
        m_CurrentScene->HandleEvent(event);
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

Viewport& Game::GetViewport()
{
    return m_Viewport;
}

GameState& Game::GetState()
{
    return m_State;
}
