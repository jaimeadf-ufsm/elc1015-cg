#include "PathTransformer.h"
#include "Intersection.h"

void PathTransformer::Offset(Path& destination, const Path& source, float amount)
{
    static std::vector<std::pair<Vector2, Vector2>> offsetEdges;

    const std::vector<Vector2>& points = source.GetPoints();

    offsetEdges.clear();
    destination.Clear();

    for (size_t i = 1; i <= points.size(); i++)
    {
        Vector2 start = points[i - 1];
        Vector2 end = points[i % points.size()];

        Vector2 direction = (start - end);
        Vector2 normal = Vector2(-direction.Y, direction.X).Normalized();
        Vector2 extrude = normal * amount;

        if (direction.Magnitude() < 0.1f)
        {
            continue;
        }

        offsetEdges.emplace_back(start + extrude, end + extrude);
    }

    auto previousEdge = offsetEdges.back();

    destination.MoveTo(previousEdge.second);

    for (size_t i = 0; i < offsetEdges.size(); i++)
    {
        auto currentEdge = offsetEdges[i];

        Vector2 intersection;

        bool intersects = IntersectSegments(
            intersection,
            previousEdge.first,
            previousEdge.second,
            currentEdge.first,
            currentEdge.second
        );

        if (intersects)
        {
            destination.LineTo(intersection);
        }
        else
        {
            destination.LineTo(previousEdge.second);
            destination.LineTo(currentEdge.first);
        }

        previousEdge = currentEdge;
    }

    if (source.IsClosed())
    {
        destination.Close();
    }
}