#include "Math.h"

bool IsPointInRectangle(const Vector2& point, const Vector2& position, const Vector2& size) {
    return (
        point.X >= position.X && point.X <= position.X + size.X &&
        point.Y >= position.Y && point.Y <= position.Y + size.Y
    );
}

bool IsPointInCircle(const Vector2& point, const Vector2& center, float radius) {
    Vector2 distance = point - center;

    return distance.MagnitudeSquared() <= radius * radius;
}