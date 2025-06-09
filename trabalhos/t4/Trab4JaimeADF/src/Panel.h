#pragma once

#include <cmath>

#include "Math.h"
#include "Event.h"
#include "Time.h"
#include "Window.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Graphics.h"
#include "GlobalContext.h"

class Panel
{
public:
    virtual void Process(const Event& event) = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;

    Vector2 GetSize() const;
    Vector2 GetPosition() const;

    void SetSize(const Vector2& size);
    void SetPosition(const Vector2& position);

    bool IsPointInside(const Vector2& point) const;

private:
    Vector2 m_Size;
    Vector2 m_Position;
};