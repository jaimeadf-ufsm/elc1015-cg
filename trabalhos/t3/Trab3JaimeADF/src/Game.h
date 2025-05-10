#pragma once

#include <memory>

#include "Event.h"
#include "DrawingContext.h"
#include "Scene.h"

#include "Mouse.h"
#include "Keyboard.h"

class Game
{
public:
    Game();

    void Update(float deltaTime);
    void Render(DrawingContext& context);

    void HandleEvent(const Event& event);

private:
    std::shared_ptr<Scene> m_CurrentScene;

    std::shared_ptr<Mouse> m_Mouse;
    std::shared_ptr<Keyboard> m_Keyboard;
};