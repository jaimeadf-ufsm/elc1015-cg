#pragma once

#include "Event.h"
#include "DrawingContext.h"

class Scene
{
public:
    virtual void Update(float deltaTime) = 0;
    virtual void Render(DrawingContext& context) = 0;
    virtual void HandleEvent(const Event& event) = 0;
};