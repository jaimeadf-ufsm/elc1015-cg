#include "Mesh.h"
#include "Math.h"

void Mesh::Clear()
{
    Vertices.clear();
    Indices.clear();
}

void Mesh::GenerateArrow(Mesh &mesh, int arcSegments)
{
    PolyLine polyline;
    polyline.InsertPoint(Vector2(0.0f, -1.0f));
    polyline.InsertPoint(Vector2(0.3f, -1.0f));
    polyline.InsertPoint(Vector2(0.3f, 0.2f));
    polyline.InsertPoint(Vector2(0.6f, 0.2f));
    polyline.InsertPoint(Vector2(0.0f, 1.0f));

    Mesh::GenerateRevolution(mesh, polyline, arcSegments);
}

void Mesh::GenerateSphere(Mesh &mesh, int xSegments, int ySegments)
{
    mesh.Clear();

    for (int y = 0; y <= ySegments; ++y)
    {
        for (int x = 0; x <= xSegments; ++x)
        {
            float xSegmentRatio = (float)x / (float)xSegments;
            float ySegmentRatio = (float)y / (float)ySegments;

            float xPos = std::cos(xSegmentRatio * MATH_TAU) * std::sin(ySegmentRatio * MATH_PI);
            float yPos = std::cos(ySegmentRatio * MATH_PI);
            float zPos = std::sin(xSegmentRatio * MATH_TAU) * std::sin(ySegmentRatio * MATH_PI);

            Vertex vertex = {
                .Position = Vector3(xPos, yPos, zPos),
                .UV = Vector2(xSegmentRatio, ySegmentRatio),
                .Normal = Vector3(xPos, yPos, zPos).Normalize(),
            };

            mesh.Vertices.emplace_back(vertex);
        }
    }

    for (int y = 0; y < ySegments; ++y)
    {
        for (int x = 0; x < xSegments; ++x)
        {
            mesh.Indices.push_back(y * (xSegments + 1) + x);
            mesh.Indices.push_back((y + 1) * (xSegments + 1) + x);
            mesh.Indices.push_back(y * (xSegments + 1) + x + 1);

            mesh.Indices.push_back(y * (xSegments + 1) + x + 1);
            mesh.Indices.push_back((y + 1) * (xSegments + 1) + x);
            mesh.Indices.push_back((y + 1) * (xSegments + 1) + x + 1);
        }
    }
}

void Mesh::GenerateRevolution(Mesh &mesh, const PolyLine &polyline, int arcSegments)
{
    mesh.Clear();

    if (polyline.GetSize() < 2 || arcSegments < 3)
    {
        return;
    }

    float angleStep = 2.0f * MATH_PI / arcSegments;

    int n = arcSegments + 1;
    int m = polyline.GetSize();

    for (int i = 0; i < n; ++i)
    {
        float theta = i * angleStep;
        float cosTheta = std::cos(theta);
        float sinTheta = std::sin(theta);

        for (int j = 0; j < m; ++j)
        {
            const Vector2 &point = polyline.GetPoint(j);

            Vertex vertex = {
                .Position = Vector3(
                    point.X * cosTheta,
                    point.Y,
                    point.X * sinTheta),
                .UV = Vector2(
                    static_cast<float>(i) / (n - 1),
                    static_cast<float>(j) / (m - 1)
                ),
                .Normal = Vector3(0.0f, 0.0f, 0.0f),
            };

            mesh.Vertices.emplace_back(vertex);
        }
    }

    for (int i = 0; i < n; ++i)
    {
        float theta = i * angleStep;
        float cosTheta = std::cos(theta);
        float sinTheta = std::sin(theta);

        for (int j = 0; j < m; ++j)
        {
            const Vector2& point = polyline.GetPoint(j);
            
            Vector2 profileTangent;

            if (j == 0)
            {
                profileTangent = polyline.GetPoint(j + 1) - point;
            }
            else if (j == m - 1)
            {
                profileTangent = point - polyline.GetPoint(j - 1);
            }
            else
            {
                profileTangent = (polyline.GetPoint(j + 1) - polyline.GetPoint(j - 1)) * 0.5f;
            }

            Vector3 axialTangent = Vector3(
                profileTangent.X * cosTheta,
                profileTangent.Y,
                profileTangent.X * sinTheta
            );

            Vector3 circumferentialTangent = Vector3(
                -point.X * sinTheta,
                0.0f,
                point.X * cosTheta
            );

            Vector3 normal = axialTangent.Cross(circumferentialTangent).Normalize();
            Vector3 radialDirection = Vector3(cosTheta, 0.0f, sinTheta);

            if (normal.Dot(radialDirection) < 0.0f)
            {
                normal = normal * -1.0f;
            }

            mesh.Vertices[i * m + j].Normal = normal;
        }
    }

    // D . ----- . C
    //   |     / |
    //   |   /   |
    //   | /     |
    // A . ----- . B

    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < m - 1; ++j)
        {
            std::size_t a = i * m + j;
            std::size_t b = a + m;
            std::size_t c = b + 1;
            std::size_t d = a + 1;

            mesh.Indices.push_back(a);
            mesh.Indices.push_back(b);
            mesh.Indices.push_back(c);

            mesh.Indices.push_back(a);
            mesh.Indices.push_back(c);
            mesh.Indices.push_back(d);
        }
    }
}