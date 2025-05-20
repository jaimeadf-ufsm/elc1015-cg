#pragma once

#include <memory>

#include "Event.h"
#include "DrawingContext.h"

#include "Mouse.h"
#include "Keyboard.h"
#include "Viewport.h"
#include "GameState.h"

class Scene;

class Game
{
public:
    Game();

    void Update(float deltaTime);
    void Draw(DrawingContext& context);

    void HandleEvent(const Event& event);

    template <typename T>
    std::shared_ptr<T> SwitchToScene();

    Mouse& GetMouse();
    Keyboard& GetKeyboard();
    Viewport& GetViewport();

    GameState &GetState();

private:
    Mouse m_Mouse;
    Keyboard m_Keyboard;
    Viewport m_Viewport;
    
    GameState m_State;

    std::shared_ptr<Scene> m_CurrentScene;
    std::shared_ptr<Scene> m_NextScene;
};

template <typename T>
std::shared_ptr<T> Game::SwitchToScene()
{
    std::shared_ptr<T> scene = std::make_shared<T>(*this);

    m_NextScene = scene;

    return scene;
}