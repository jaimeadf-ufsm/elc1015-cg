#pragma once

#include "Panel.h"
#include "Renderer.h"
#include "Camera.h"
#include "ModelShader.h"

class PreviewPanel : public Panel
{
public:
    PreviewPanel();

    void Process(const Event& event) override;
    void Update() override;
    void Draw() override;

private:
    enum class ProjectionType
    {
        Perspective,
        Orthographic
    };

    enum class ShaderType
    {
        Wireframe,
        Flat,
        Phong
    };

    enum class Target
    {
        ModelPosition,
        ModelRotation,
        Light
    };

    Renderer m_Renderer;
    Camera m_Camera;

    Matrix4x4 m_ProjectionMatrix;
    Matrix4x4 m_ViewMatrix;

    Mesh m_SphereMesh;
    Mesh m_ArrowMesh;

    Vector3 m_LightPosition;

    Vector3 m_ModelPosition;
    Vector3 m_ModelRotation;

    ProjectionType m_ProjectionType;
    ShaderType m_ShaderType;
    Target m_Target;

    Sampler m_Texture;

    bool m_ShowNormals;

    bool m_MouseDragging;
    Vector2 m_MouseLastPosition;

    void RenderLight();
    void RenderModel();
    void RenderNormals();

    void DrawTarget();

    void ResetScene();
    void UpdateProjectionMatrix();
};
