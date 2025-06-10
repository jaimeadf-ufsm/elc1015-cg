#include "Mesh.h"
#include "Math.h"

void Mesh::Clear()
{
    Vertices.clear();
    Indices.clear();
}

void Mesh::GenerateArrow(Mesh &mesh, int xResolution)
{
    PolyLine2D polyline;
    polyline.InsertPoint(Vector2(0.0f, -1.0f));
    polyline.InsertPoint(Vector2(0.3f, -1.0f));
    polyline.InsertPoint(Vector2(0.3f, 0.2f));
    polyline.InsertPoint(Vector2(0.6f, 0.2f));
    polyline.InsertPoint(Vector2(0.0f, 1.0f));

    Mesh::GenerateRevolution(mesh, polyline, xResolution);
}

void Mesh::GenerateSphere(Mesh &mesh, int xResolution, int yResolution)
{
    mesh.Clear();

    for (int y = 0; y <= yResolution; ++y)
    {
        for (int x = 0; x <= xResolution; ++x)
        {
            float xSegmentRatio = (float)x / (float)xResolution;
            float ySegmentRatio = (float)y / (float)yResolution;

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

    for (int y = 0; y < yResolution; ++y)
    {
        for (int x = 0; x < xResolution; ++x)
        {
            mesh.Indices.push_back(y * (xResolution + 1) + x);
            mesh.Indices.push_back(y * (xResolution + 1) + x + 1);
            mesh.Indices.push_back((y + 1) * (xResolution + 1) + x);

            mesh.Indices.push_back((y + 1) * (xResolution + 1) + x);
            mesh.Indices.push_back(y * (xResolution + 1) + x + 1);
            mesh.Indices.push_back((y + 1) * (xResolution + 1) + x + 1);
        }
    }
}

void Mesh::GenerateRevolution(Mesh &mesh, const PolyLine2D &polyline, int resolution)
{
    mesh.Clear();

    if (polyline.GetSize() < 2 || resolution < 3)
    {
        return;
    }

    float angleStep = 2.0f * MATH_PI / resolution;

    int n = resolution + 1;
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

            mesh.Vertices[i * m + j].Normal = normal;
        }
    }    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < m - 1; ++j)
        {
            std::size_t a = i * m + j;
            std::size_t b = a + m;
            std::size_t c = b + 1;
            std::size_t d = a + 1;

            mesh.Indices.push_back(a);
            mesh.Indices.push_back(d);
            mesh.Indices.push_back(b);

            mesh.Indices.push_back(b);
            mesh.Indices.push_back(d);
            mesh.Indices.push_back(c);
        }
    }
}

void Mesh::GenerateSweep(Mesh &mesh, const PolyLine2D& shape, const PolyLine3D& path)
{
    mesh.Clear();
    
    if (path.GetSize() < 2 || shape.GetSize() < 2)
    {
        return;
    }
    
    int pathSize = path.GetSize();
    int shapeSize = shape.GetSize();

    if (shape.GetPoint(0) == shape.GetPoint(shapeSize - 1))
    {
        --shapeSize;
    }
    
    for (int i = 0; i < pathSize; ++i)
    {
        const Vector3& pathPoint = path.GetPoint(i);
        
        Vector3 tangent;
        if (i == 0)
        {
            tangent = path.GetPoint(i + 1) - pathPoint;
        }
        else if (i == pathSize - 1)
        {
            tangent = pathPoint - path.GetPoint(i - 1);
        }
        else
        {
            tangent = (path.GetPoint(i + 1) - path.GetPoint(i - 1)) * 0.5f;
        }
        tangent = tangent.Normalize();
        
        Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
        Vector3 right;
        Vector3 forward = tangent;
        
        if (std::abs(forward.Dot(up)) > 0.99f)
        {
            up = Vector3(1.0f, 0.0f, 0.0f);
        }
        
        right = forward.Cross(up).Normalize();
        up = right.Cross(forward).Normalize();
        
        for (int j = 0; j < shapeSize; ++j)
        {
            const Vector2& shapePoint = shape.GetPoint(j);
            
            Vector3 worldPosition = pathPoint + 
                                  right * shapePoint.X + 
                                  up * shapePoint.Y;
            
            Vertex vertex = {
                .Position = worldPosition,
                .UV = Vector2(
                    static_cast<float>(i) / (pathSize - 1),
                    static_cast<float>(j) / (shapeSize - 1)
                ),
                .Normal = Vector3(0.0f, 0.0f, 0.0f),
            };
            
            mesh.Vertices.emplace_back(vertex);
        }
    }
    
    for (int i = 0; i < pathSize; ++i)
    {
        for (int j = 0; j < shapeSize; ++j)
        {
            Vector3 normal = Vector3(0.0f, 0.0f, 0.0f);
            int vertexIndex = i * shapeSize + j;
            
            Vector3 tangentU, tangentV;
            
            if (i == 0)
            {
                tangentU = mesh.Vertices[(i + 1) * shapeSize + j].Position - 
                          mesh.Vertices[i * shapeSize + j].Position;
            }
            else if (i == pathSize - 1)
            {
                tangentU = mesh.Vertices[i * shapeSize + j].Position - 
                          mesh.Vertices[(i - 1) * shapeSize + j].Position;
            }
            else
            {
                tangentU = (mesh.Vertices[(i + 1) * shapeSize + j].Position - 
                           mesh.Vertices[(i - 1) * shapeSize + j].Position) * 0.5f;
            }
            
            int nextJ = (j + 1) % shapeSize;
            int prevJ = (j - 1 + shapeSize) % shapeSize;
            
            tangentV = mesh.Vertices[i * shapeSize + nextJ].Position - 
                      mesh.Vertices[i * shapeSize + prevJ].Position;
            
            normal = tangentU.Cross(tangentV).Normalize();
            
            mesh.Vertices[vertexIndex].Normal = normal;
        }
    }
    
    for (int i = 0; i < pathSize - 1; ++i)
    {
        for (int j = 0; j < shapeSize; ++j)
        {
            int nextJ = (j + 1) % shapeSize;
            
            std::size_t a = i * shapeSize + j;
            std::size_t b = i * shapeSize + nextJ;
            std::size_t c = (i + 1) * shapeSize + nextJ;
            std::size_t d = (i + 1) * shapeSize + j;
            
            mesh.Indices.push_back(a);
            mesh.Indices.push_back(d);
            mesh.Indices.push_back(c);
            
            mesh.Indices.push_back(a);
            mesh.Indices.push_back(c);
            mesh.Indices.push_back(b);
        }
    }
}
