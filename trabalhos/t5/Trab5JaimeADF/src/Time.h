#pragma once

#include <chrono>

class Time
{
public:
    static float DeltaTime;

    static void Update()
    {
        static std::chrono::high_resolution_clock::time_point lastTime;

        if (lastTime.time_since_epoch().count() == 0)
        {
            lastTime = std::chrono::high_resolution_clock::now();
        }

        auto currentTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> elapsed = currentTime - lastTime;

        DeltaTime = elapsed.count();

        if (DeltaTime < 0.0f)
        {
            DeltaTime = 0.0f;
        }

        lastTime = currentTime;
    }
};

float Time::DeltaTime = 0.0f;