#pragma once

#include <vector>
#include <cstdint>

#include "Vector2.h"

class PolyLine
{
public:
    PolyLine();

    void AddPoint(const Vector2& point);

    void Close();
    void Clear();

    bool IsClosed() const;
    const std::vector<Vector2>& GetPoints() const;

private:
    bool m_Closed = false;
    std::vector<Vector2> m_Points;
};
