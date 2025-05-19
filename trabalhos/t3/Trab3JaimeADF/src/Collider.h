#pragma once

#include <vector>
#include <memory>

#include "Vector2.h"
#include "Mesh.h"

enum ColliderMode
{
    Dynamic,
    Static,
    Sensor
};

class Collider
{
public:
    Collider();

    void AddVertex(Vector2 point);
    void RemoveVertex(size_t index);

    void SetMode(ColliderMode mode);
    void SetPolygon(std::vector<Vector2>&& vertices);

    ColliderMode GetMode() const;
    const std::vector<Vector2>& GetPolygon() const;

    bool IsDynamic() const;
    bool IsStatic() const;
    bool IsSensor() const;

    static std::vector<std::shared_ptr<Collider>> Box(
        const Vector2& size,
        ColliderMode mode = ColliderMode::Dynamic
    );

    static std::vector<std::shared_ptr<Collider>> Circle(
        float radius,
        ColliderMode mode = ColliderMode::Dynamic
    );

    static std::vector<std::shared_ptr<Collider>> Meshes(
        const std::vector<Mesh>& mesh,
        ColliderMode mode = ColliderMode::Dynamic
    );

private:
    ColliderMode m_Mode;
    std::vector<Vector2> m_Polygon;
};