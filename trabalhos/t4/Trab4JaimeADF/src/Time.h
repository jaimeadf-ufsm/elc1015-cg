/**
 * @file Time.h
 * @brief Time management class for delta time tracking
 */

#pragma once

/**
 * @class Time
 * @brief Static class for managing frame time and delta time calculations
 */
class Time
{
public:
    static void SetDeltaTime(float deltaTime);
    static float GetDeltaTime();

private:
    static float s_DeltaTime;
};
