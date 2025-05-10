#include "Game.h"

#include "EditorScene.h"

Game::Game()
{
    m_Mouse = std::make_shared<Mouse>();
    m_Keyboard = std::make_shared<Keyboard>();
    m_CurrentScene = std::make_shared<EditorScene>(m_Mouse, m_Keyboard);
}

void Game::Update(float deltaTime)
{
    m_CurrentScene->Update(deltaTime);
}

void Game::Render(DrawingContext& context)
{
    m_CurrentScene->Render(context);
}

void Game::HandleEvent(const Event& event)
{
    switch (event.Type)
    {
        case EventType::MouseMove:
            m_Mouse->SetPosition(event.Mouse.Position);
            break;
        case EventType::MouseButtonPress:
            m_Mouse->SetButtonState(event.Mouse.Button, true);
            break;
        case EventType::MouseButtonRelease:
            m_Mouse->SetButtonState(event.Mouse.Button, false);
            break;
        case EventType::KeyPress:
            m_Keyboard->SetKeyState(event.Key.Key, true);
            break;
        case EventType::KeyRelease:
            m_Keyboard->SetKeyState(event.Key.Key, false);
            break;
    }

    m_CurrentScene->HandleEvent(event);
}