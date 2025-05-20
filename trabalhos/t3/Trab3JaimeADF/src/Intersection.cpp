#include "Intersection.h"

bool IntersectSegments(Vector2 &result, Vector2 a, Vector2 b, Vector2 c, Vector2 d)
{
    float den = (d.Y - c.Y) * (b.X - a.X) - (d.X - c.X) * (b.Y - a.Y);

    if (den == 0)
    {
        return false;
    }

    float ua = ((d.X - c.X) * (a.Y - c.Y) - (d.Y - c.Y) * (a.X - c.X)) / den;
    float ub = ((b.X - a.X) * (a.Y - c.Y) - (b.Y - a.Y) * (a.X - c.X)) / den;

    bool isIntersectionOutside = ua < 0 || ub < 0 || ua > 1 || ub > 1;

    result.X = a.X + ua * (b.X - a.X);
    result.Y = a.Y + ua * (b.Y - a.Y);

    return isIntersectionOutside;
}