#include <algorithm>
#include <cmath>
#include <iostream>

#include "Renderer.h"

inline Vector3 ComputeBarycentricCoordinates(const Vector3& p, const Vector3& v0, const Vector3& v1, const Vector3& v2)
{
    float denominator = (v1.Y - v2.Y) * (v0.X - v2.X) + (v2.X - v1.X) * (v0.Y - v2.Y);
    
    if (std::abs(denominator) < 1e-6f)
    {
        return Vector3(0.0f, 0.0f, 0.0f);
    }
    
    float a = ((v1.Y - v2.Y) * (p.X - v2.X) + (v2.X - v1.X) * (p.Y - v2.Y)) / denominator;
    float b = ((v2.Y - v0.Y) * (p.X - v2.X) + (v0.X - v2.X) * (p.Y - v2.Y)) / denominator;
    float c = 1.0f - a - b;
    
    return Vector3(a, b, c);
}

Renderer::Renderer() : m_FaceCulling(true), m_FrameBuffer(0, 0), m_DepthBuffer()
{
}

void Renderer::Render(const Mesh& mesh, const Shader& shader)
{
    int width = m_FrameBuffer.GetWidth();
    int height = m_FrameBuffer.GetHeight();

    m_Triangles.clear();

    for (std::size_t i = 0; i + 3 <= mesh.Indices.size(); i += 3)
    {
        const Vertex& v0 = mesh.Vertices[mesh.Indices[i]];
        const Vertex& v1 = mesh.Vertices[mesh.Indices[i + 1]];
        const Vertex& v2 = mesh.Vertices[mesh.Indices[i + 2]];

        shader.ProcessGeometry(v0, v1, v2, m_Triangles);
    }

    for (std::size_t i = 0; i + 3 <= m_Triangles.size(); i += 3)
    {
        const VertexInput& vi0 = m_Triangles[i];
        const VertexInput& vi1 = m_Triangles[i + 1];
        const VertexInput& vi2 = m_Triangles[i + 2];

        VertexOutput vo0 = shader.ProcessVertex(vi0);
        VertexOutput vo1 = shader.ProcessVertex(vi1);
        VertexOutput vo2 = shader.ProcessVertex(vi2);

        // Clip triangles that are behind the camera or have invalid W values
        if (vo0.ClipPosition.W <= 1e-6f || vo1.ClipPosition.W <= 1e-6f || vo2.ClipPosition.W <= 1e-6f)
        {
            continue;
        }

        Vector3 p0 = GetScreenCoordinates(vo0.ClipPosition);
        Vector3 p1 = GetScreenCoordinates(vo1.ClipPosition);
        Vector3 p2 = GetScreenCoordinates(vo2.ClipPosition);

        Vector3 edge1 = p1 - p0;
        Vector3 edge2 = p2 - p0;

        float crossZ = Vector2(edge1.X, edge1.Y).Cross(Vector2(edge2.X, edge2.Y));
        
        if (std::abs(crossZ) < 1e-6f)
        {
            continue;
        }
        
        if (m_FaceCulling && crossZ > 0.0f)
        {
            continue;
        }

        int minX = static_cast<int>(std::floor(std::min({ p0.X, p1.X, p2.X })));
        int minY = static_cast<int>(std::floor(std::min({ p0.Y, p1.Y, p2.Y })));
        int maxX = static_cast<int>(std::ceil(std::max({ p0.X, p1.X, p2.X })));
        int maxY = static_cast<int>(std::ceil(std::max({ p0.Y, p1.Y, p2.Y })));

        minX = std::max(minX, 0);
        minY = std::max(minY, 0);
        maxX = std::min(maxX, width - 1);
        maxY = std::min(maxY, height - 1);

        for (int y = minY; y <= maxY; ++y)
        {
              for (int x = minX; x <= maxX; ++x)
            {
                float px = x + 0.5f;
                float py = y + 0.5f;
                
                Vector3 barycentric = ComputeBarycentricCoordinates(Vector3(px, py, 0.0f), p0, p1, p2);

                if (barycentric.X < 0.0f || barycentric.Y < 0.0f || barycentric.Z < 0.0f)
                {
                    continue;
                }                float z = barycentric.X * p0.Z + barycentric.Y * p1.Z + barycentric.Z * p2.Z;

                // More robust depth testing
                if (z < -1.0f || z > 1.0f || !std::isfinite(z))
                {
                    continue;
                }

                std::size_t index = y * width + x;

                if (z >= m_DepthBuffer[index])
                {
                    continue;
                }

                FragmentInput fi;
                fi.Barycentric = barycentric;
                fi.ScreenCoordinate = Vector2(px, py);
                fi.WorldPosition = Interpolate(vo0.WorldPosition, vo1.WorldPosition, vo2.WorldPosition, barycentric);
                fi.UV = Interpolate(vo0.UV, vo1.UV, vo2.UV, barycentric);
                fi.Normal = Interpolate(vo0.Normal, vo1.Normal, vo2.Normal, barycentric);

                FragmentOutput fo = shader.ProcessFragment(fi);
                
                if (fo.Color.W != 0.0f)
                {
                    m_DepthBuffer[index] = z;
                    m_FrameBuffer.SetPixel(x, y, ColorRGB(fo.Color.X, fo.Color.Y, fo.Color.Z));
                }
            }
        }
    }
}

const Image& Renderer::GetFrameBuffer() const
{
    return m_FrameBuffer;
}

void Renderer::Resize(std::size_t width, std::size_t height)
{
    m_FrameBuffer.Resize(width, height);
    m_DepthBuffer.resize(width * height, 1.0f);
}

void Renderer::ClearFrameBuffer(const ColorRGB& color)
{
    m_FrameBuffer.Clear(color);
}

void Renderer::ClearDepthBuffer(float depth)
{
    std::fill(m_DepthBuffer.begin(), m_DepthBuffer.end(), depth);
}

Vector3 Renderer::GetScreenCoordinates(const Vector4& point) const
{
    Vector3 cartesianPoint = point.ToCartesian();

    return Vector3(
        (cartesianPoint.X + 1.0f) * 0.5f * m_FrameBuffer.GetWidth(),
        (cartesianPoint.Y + 1.0f) * 0.5f * m_FrameBuffer.GetHeight(),
        cartesianPoint.Z
    );
}

void Renderer::SetFaceCulling(bool enabled)
{
    m_FaceCulling = enabled;
}

bool Renderer::IsFaceCulling() const
{
    return m_FaceCulling;
}