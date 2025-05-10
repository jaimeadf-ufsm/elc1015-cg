#include "Tesselator.h"

#include <algorithm>

bool IsConvex(const Triangle& triangle)
{
    Vector2 ab = triangle.B - triangle.A;
    Vector2 ac = triangle.C - triangle.A;

    return ab.Cross(ac) > 0.0f;
}

bool IsEar(const std::vector<Vector2>& vertices, size_t index, Triangle &triangle)
{
    size_t ia = (index + vertices.size() - 1) % vertices.size();
    size_t ib = index;
    size_t ic = (index + 1) % vertices.size();

    Vector2 a = vertices[ia];
    Vector2 b = vertices[ib];
    Vector2 c = vertices[ic];

    triangle = Triangle(a, b, c);

    if (!IsConvex(triangle))
    {
        return false;
    }

    for (size_t j = 0; j < vertices.size(); j++)
    {
        if (j != ia && j != ib && j != ic && triangle.Contains(vertices[j]))
        {
            return false;
        }
    }

    return true;
}

Vector2 SolveIntersection(const Vector2& a, const Vector2& v, const Vector2& b, const Vector2& u)
{
    float denom = v.X * u.Y - v.Y * u.X;

    if (denom == 0.0f)
    {
        return Vector2(0, 0);
    }

    float t = ((b.X - a.X) * u.Y - (b.Y - a.Y) * u.X) / denom;

    return a + v * t;
}


void Tesselator::Stroke(const Path& path, std::vector<Triangle>& triangles, float width, float miterLimit)
{
    triangles.clear();

    float halfWidth = width / 2.0f;

    const std::vector<Vector2>& points = path.GetPoints();

    if (points.size() < 2)
    {
        return;
    }

    size_t s = path.IsClosed() ? 0 : 1;
    size_t n = path.IsClosed() ? points.size() : points.size() - 1;

    for (size_t i = 0; i < n; i++)
    {
        Vector2 a = points[i];
        Vector2 b = points[(i + 1) % points.size()];

        Vector2 direction = (b - a).Normalize();
        Vector2 extrude = Vector2(-direction.Y, direction.X) * halfWidth;

        Vector2 p1 = a + extrude;
        Vector2 p2 = a - extrude;
        Vector2 p3 = b + extrude;
        Vector2 p4 = b - extrude;

        triangles.emplace_back(p1, p3, p4);
        triangles.emplace_back(p1, p4, p2);
    }

    for (size_t i = s; i < n; i++)
    {
        Vector2 a = points[(i + points.size() - 1) % points.size()];
        Vector2 b = points[i];
        Vector2 c = points[(i + 1) % points.size()];

        Vector2 previousDirection = (b - a).Normalize();
        Vector2 nextDirection = (c - b).Normalize();

        Vector2 previousExtrude = Vector2(-previousDirection.Y, previousDirection.X) * halfWidth;
        Vector2 nextExtrude = Vector2(-nextDirection.Y, nextDirection.X) * halfWidth;

        float result = previousDirection.Cross(nextDirection);
        
        if (result == 0.0f)
        {
            continue;
        }
        else if (result > 0.0f)
        {
            previousExtrude = -previousExtrude;
            nextExtrude = -nextExtrude;
        }

        Vector2 previousPoint = b + previousExtrude;
        Vector2 nextPoint = b + nextExtrude;

        Vector2 miterPoint = SolveIntersection(previousPoint, previousDirection, nextPoint, nextDirection);
        float miterLength = (miterPoint - b).Magnitude();

        if (miterLength > miterLimit * halfWidth)
        {
            triangles.emplace_back(previousPoint, b, nextPoint);
        }
        else
        {
            triangles.emplace_back(previousPoint, b, miterPoint);
            triangles.emplace_back(b, miterPoint, nextPoint);
        }
    }
}

void Tesselator::Fill(const Path& path, std::vector<Triangle>& triangles)
{
    triangles.clear();
    
    std::vector<Vector2> vertices = path.GetPoints();

    while (vertices.size() > 3)
    {
        for (size_t i = 0; i < vertices.size(); i++)
        {
            Triangle triangle;

            if (IsEar(vertices, i, triangle))
            {
                triangles.push_back(triangle);
                vertices.erase(vertices.begin() + i);

                break;
            }
        }
    }

    if (vertices.size() == 3)
    {
        triangles.push_back(Triangle(vertices[0], vertices[1], vertices[2]));
    }
}
