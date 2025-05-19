#pragma once

#include "Vector2.h"

class BoundingBox
{
public:
    BoundingBox();

    void Encompass(const Vector2& point);

    Vector2 GetMinPoint() const;
    Vector2 GetMaxPoint() const;

private:
    Vector2 m_MinPoint;
    Vector2 m_MaxPoint;
};