#include <cmath>

#include "Collider.h"
#include "Constants.h"

Collider::Collider()
{
}

void Collider::AddVertex(Vector2 point)
{
    m_Polygon.emplace_back(point);
}

void Collider::RemoveVertex(size_t index)
{
    m_Polygon.erase(m_Polygon.begin() + index);
}

void Collider::SetMode(ColliderMode mode)
{
    m_Mode = mode;
}

void Collider::SetPolygon(std::vector<Vector2>&& vertices)
{
    m_Polygon = std::move(vertices);
}

ColliderMode Collider::GetMode() const
{
    return m_Mode;
}

const std::vector<Vector2>& Collider::GetPolygon() const
{
    return m_Polygon;
}

bool Collider::IsDynamic() const
{
    return m_Mode == ColliderMode::Dynamic;
}

bool Collider::IsStatic() const
{
    return m_Mode == ColliderMode::Static;
}

bool Collider::IsSensor() const
{
    return m_Mode == ColliderMode::Sensor;
}

std::vector<std::shared_ptr<Collider>> Collider::Box(const Vector2& size, ColliderMode mode)
{
    float halfWidth = size.X / 2.0f;
    float halfHeight = size.Y / 2.0f;

    std::shared_ptr<Collider> collider = std::make_shared<Collider>();

    collider->SetMode(mode);

    collider->AddVertex(Vector2(-halfWidth, -halfHeight));
    collider->AddVertex(Vector2(+halfWidth, -halfHeight));
    collider->AddVertex(Vector2(+halfWidth, +halfHeight));
    collider->AddVertex(Vector2(-halfWidth, +halfHeight));

    return { collider };
}

std::vector<std::shared_ptr<Collider>> Collider::Circle(float radius, ColliderMode mode)
{
    std::shared_ptr<Collider> collider = std::make_shared<Collider>();

    int steps = (2.0f * PI * radius) / 4;
    float increment = 2 * PI / steps;

    collider->SetMode(mode);

    for (int step = 0; step < steps; step++)
    {
        float angle = step * increment;
        Vector2 direction = Vector2(std::cos(angle), std::sin(angle));

        collider->AddVertex(direction * radius);
    }

    return { collider };
}

std::vector<std::shared_ptr<Collider>> Collider::Meshes(
    const std::vector<Mesh>& meshes,
    ColliderMode mode
)
{
    std::vector<std::shared_ptr<Collider>> colliders;

    for (const Mesh& mesh : meshes)
    {
        for (size_t i = 0; i + 3 <= mesh.Triangles.size(); i += 3)
        {
            size_t indexA = mesh.Triangles[i];
            size_t indexB = mesh.Triangles[i + 1];
            size_t indexC = mesh.Triangles[i + 2];

            Vector2 vertexA = mesh.Vertices[indexA];
            Vector2 vertexB = mesh.Vertices[indexB];
            Vector2 vertexC = mesh.Vertices[indexC];

            std::shared_ptr<Collider> collider = std::make_shared<Collider>();

            collider->SetMode(mode);

            collider->AddVertex(vertexA);
            collider->AddVertex(vertexB);
            collider->AddVertex(vertexC);

            colliders.emplace_back(collider);
        }
    }

    return colliders;
}