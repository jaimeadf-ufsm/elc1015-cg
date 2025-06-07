#pragma once

#include <vector>

#include "Vector2.h"

class PolyLine
{
public:
    void InsertPoint(const Vector2& point);
    void RemovePoint(std::size_t index);

    Vector2& GetPoint(std::size_t index);
    const Vector2& GetPoint(std::size_t index) const;

    std::size_t GetSize() const;
    
    void Clear();

private:
    std::vector<Vector2> m_Points;
};

