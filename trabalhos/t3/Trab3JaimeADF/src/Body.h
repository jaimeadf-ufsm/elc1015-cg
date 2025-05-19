#pragma once

#include "Vector2.h"

class Body
{
public:
    Body();

    void SetLinearVelocity(Vector2 velocity);
    void SetAngularVelocity(float velocity);

    Vector2 GetLinearVelocity();
    float GetAngularVelocity();

private:
    Vector2 m_LinearVelocity;
    float m_AngularVelocity;
};