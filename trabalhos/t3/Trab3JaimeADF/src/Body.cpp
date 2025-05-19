#include "Body.h"

Body::Body() : m_LinearVelocity(), m_AngularVelocity()
{

}

void Body::SetLinearVelocity(Vector2 velocity)
{
    m_LinearVelocity = velocity;
}

void Body::SetAngularVelocity(float velocity)
{
    m_AngularVelocity = velocity;
}

Vector2 Body::GetLinearVelocity()
{
    return m_LinearVelocity;
}

float Body::GetAngularVelocity()
{
    return m_AngularVelocity;
}
