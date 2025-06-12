/**
 * @file PolyLine.h
 * @brief Template class for polylines (sequences of connected points)
 */

#pragma once

#include <vector>

#include "Vector2.h"
#include "Vector3.h"

/**
 * @class PolyLine
 * @brief Template class for managing sequences of connected points
 */
template<typename T>
class PolyLine
{
public:
    void InsertPoint(const T& point)
    {
        m_Points.push_back(point);
    }
    
    void RemovePoint(std::size_t index)
    {
        m_Points.erase(m_Points.begin() + index);
    }

    T& GetPoint(std::size_t index)
    {
        return m_Points.at(index);
    }

    const T& GetPoint(std::size_t index) const
    {
        return m_Points.at(index);
    }

    std::size_t GetSize() const
    {
        return m_Points.size();
    }

    void Clear()
    {
        m_Points.clear();
    }

private:
    std::vector<T> m_Points;
};

using PolyLine2D = PolyLine<Vector2>;
using PolyLine3D = PolyLine<Vector3>;
