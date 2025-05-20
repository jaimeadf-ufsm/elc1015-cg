#pragma once

#include "Scene.h"

class EditorScene : public Scene
{
public:
    EditorScene(std::reference_wrapper<Game> game);

    void Start();
    void Stop();

    void Update(float deltaTime);
    void Draw(DrawingContext& context);

    void HandleEvent(const Event& event);
};