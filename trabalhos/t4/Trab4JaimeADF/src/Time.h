#pragma once

class Time
{
public:
    static void SetDeltaTime(float deltaTime);
    static float GetDeltaTime();

private:
    static float s_DeltaTime;
};
