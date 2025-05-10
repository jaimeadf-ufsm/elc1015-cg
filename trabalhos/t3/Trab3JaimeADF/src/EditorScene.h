#pragma once

#include <memory>

#include "Scene.h"
#include "Mouse.h"
#include "Keyboard.h"

class EditorScene : public Scene
{
public:
    EditorScene(std::shared_ptr<Mouse> mouse, std::shared_ptr<Keyboard> keyboard);

    void Update(float deltaTime) override;
    void Render(DrawingContext& context) override;
    void HandleEvent(const Event& event) override;

private:
    std::vector<Vector2> m_Points;

    std::shared_ptr<Mouse> m_Mouse;
    std::shared_ptr<Keyboard> m_Keyboard;

    bool m_Fill = false;

    int m_SelectedPoint = -1;
};
