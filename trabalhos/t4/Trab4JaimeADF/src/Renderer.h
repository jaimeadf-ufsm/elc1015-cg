#pragma once

#include "Image.h"
#include "Mesh.h"
#include "Shader.h"
#include "Matrix4x4.h"

class Renderer
{
public:
    Renderer();

    const Image& GetFrameBuffer() const;

    void Render(const Mesh& mesh, const Shader& shader);

    void Resize(std::size_t width, std::size_t height);

    void ClearFrameBuffer(const ColorRGB& color = ColorRGB(0, 0, 0));
    void ClearDepthBuffer(float depth = 1.0f);

    void SetFaceCulling(bool enabled);
    bool IsFaceCulling() const;

private:
    bool m_FaceCulling;

    Image m_FrameBuffer;
    std::vector<float> m_DepthBuffer;

    std::vector<VertexInput> m_Triangles;


    Vector3 GetScreenCoordinates(const Vector4& point) const;

    template<typename T>
    T Interpolate(const T& a, const T& b, const T& c, Vector3 barycentric) const
    {
        return a * barycentric.X + b * barycentric.Y + c * barycentric.Z;
    }
};