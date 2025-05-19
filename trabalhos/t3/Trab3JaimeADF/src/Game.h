#pragma once

#include <memory>

#include "Event.h"
#include "DrawingContext.h"

#include "Mouse.h"
#include "Keyboard.h"
#include "PhysicsSystem.h"

class GameObject;

class Game
{
public:
    Game();

    void Update(float deltaTime);
    void Draw(DrawingContext& context);

    void HandleEvent(const Event& event);

    template<typename T> std::shared_ptr<T> CreateObject()
    {
        std::shared_ptr<T> object = std::make_shared<T>(std::reference_wrapper<Game>(*this));

        m_Objects.emplace_back(object);
        object->Initialize();

        return object;
    }

    void DestroyObject(std::shared_ptr<GameObject> object);

    Mouse& GetMouse();
    Keyboard& GetKeyboard();

    std::vector<std::shared_ptr<GameObject>>& GetObjects();

private:
    Mouse m_Mouse;
    Keyboard m_Keyboard;

    PhysicsSystem m_PhysicsSystem;

    std::vector<std::shared_ptr<GameObject>> m_Objects;
};