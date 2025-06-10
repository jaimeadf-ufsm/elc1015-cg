/**
 * @file PreviewPanel.h
 * @brief 3D preview panel for rendering and displaying models
 */

#pragma once

#include "Panel.h"
#include "Renderer.h"
#include "Camera.h"
#include "ModelShader.h"

/**
 * @class PreviewPanel
 * @brief Interactive 3D preview panel with camera controls and rendering options
 */
class PreviewPanel : public Panel
{
public:
    PreviewPanel();

    void Process(const Event& event) override;
    void Update() override;
    void Draw() override;

private:
    /**
     * @enum ProjectionType
     * @brief Type of projection for 3D rendering
     */
    enum class ProjectionType
    {
        Perspective,
        Orthographic    };

    /**
     * @enum ShaderType
     * @brief Type of shader for rendering
     */
    enum class ShaderType
    {
        Wireframe,
        Flat,
        Phong    };

    /**
     * @enum NormalMode
     * @brief Mode for displaying surface normals
     */
    enum class NormalMode
    {
        None,
        Vertex,
        Face    };
    
    /**
     * @enum Target
     * @brief Target for user interaction and manipulation
     */
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
    Matrix4x4 m_ModelMatrix;

    Mesh m_SphereMesh;
    Mesh m_ArrowMesh;

    Vector3 m_LightPosition;

    Vector3 m_ModelPosition;
    Vector3 m_ModelRotation;

    ProjectionType m_ProjectionType;
    ShaderType m_ShaderType;
    NormalMode m_NormalMode;
    Target m_Target;
    
    Sampler m_Texture;    bool m_MouseDragging;
    Vector2 m_MouseLastPosition;

    void RenderLight();
    void RenderModel();
    void RenderVertexNormals();
    void RenderFaceNormals();

    void DrawTarget();

    void ResetScene();
    void ResetCamera();

    void UpdateProjectionMatrix();
    void UpdateViewMatrix();
    void UpdateModelMatrix();
};
