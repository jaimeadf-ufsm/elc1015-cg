/**
 * @file Mathematics.h
 * @brief Mathematics utility header for mathematical constants and functions
 */

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include "Vector.h"

Vector3 RandomPointOnShell(float innerR, float outerR)
{
    float theta = (rand() / (float)RAND_MAX) * 2.0f * M_PI;
    float phi = (rand() / (float)RAND_MAX) * M_PI;

    float innerRCubed = innerR * innerR * innerR;
    float outerRCubed = outerR * outerR * outerR;
    float volume = (rand() / (float)RAND_MAX);
    float radiusCubed = innerRCubed + volume * (outerRCubed - innerRCubed);
    float radius = powf(radiusCubed, 1.0f / 3.0f);

    Vector3 position;
    position.X = radius * sinf(phi) * cosf(theta);
    position.Y = radius * cosf(phi);
    position.Z = radius * sinf(phi) * sinf(theta);

    return position;
}
