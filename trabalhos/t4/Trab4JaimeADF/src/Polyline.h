#pragma once

#include <vector>

#include "Vector2.h"
#include "Vector3.h"

template<typename T>
class PolyLine
{
public:
    T Evaluate(float t) const
    {
        if (m_Points.empty())
        {
            return T(0.0f, 0.0f);
        }

        if (t <= 0.0f)
        {
            return m_Points.front();
        }
        
        if (t >= static_cast<float>(m_Points.size() - 1))
        {
            return m_Points.back();
        }

        std::size_t index = static_cast<std::size_t>(t);
        float fraction = t - static_cast<float>(index);

        return Lerp(m_Points[index], m_Points[index + 1], fraction);
    }

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
