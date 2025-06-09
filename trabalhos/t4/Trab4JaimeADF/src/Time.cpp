#include "Time.h"

float Time::s_DeltaTime = 0.0f;

void Time::SetDeltaTime(float deltaTime)
{
    s_DeltaTime = deltaTime;
}

float Time::GetDeltaTime()
{
    return s_DeltaTime;
}
