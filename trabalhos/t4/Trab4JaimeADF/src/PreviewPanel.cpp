#include <iostream>

#include "PreviewPanel.h"
#include "WireframeShader.h"
#include "FlatShader.h"
#include "WireframeShader.h"
#include "PhongShader.h"
#include "GlobalContext.h"
#include "BMP.h"

PreviewPanel::PreviewPanel() : Panel()
{
    Mesh::GenerateSphere(m_SphereMesh, 32, 32);
    Mesh::GenerateArrow(m_ArrowMesh, 32);

    m_ProjectionType = ProjectionType::Perspective;
    m_ShaderType = ShaderType::Wireframe;
    m_Target = Target::ModelPosition;

    m_ShowNormals = false;

    m_MouseDragging = false;

    ResetScene();

    SetSize(Vector2(Window::GetWidth() / 2.0f, Window::GetHeight()));
    SetPosition(Vector2(Window::GetWidth() / 2.0f + 0.5f, 0.5f));
}

void PreviewPanel::Process(const Event& event)
{
    switch (event.Type)
    {
    case EventType::WindowResize:
        SetSize(Vector2(event.Window.Width / 2.0f, event.Window.Height));
        SetPosition(Vector2(event.Window.Width / 2.0f, 0.0f));
        break;
    case EventType::MouseButtonPress:
        if (IsPointInside(event.Mouse.Position))
        {
            if (event.Mouse.Button == MOUSE_BUTTON_LEFT)
            {
                m_MouseDragging = true;
                m_MouseLastPosition = event.Mouse.Position;
            }
        }
        break;
    case EventType::MouseButtonRelease:
        if (event.Mouse.Button == MOUSE_BUTTON_LEFT)
        {
            m_MouseDragging = false;
        }
        break;
    case EventType::MouseMove:
        if (IsPointInside(event.Mouse.Position) && m_MouseDragging)
        {
            Vector2 delta = event.Mouse.Position - m_MouseLastPosition;

            m_Camera.SetYaw(m_Camera.GetYaw() + delta.X * 0.01f);
            m_Camera.SetPitch(m_Camera.GetPitch() + delta.Y * 0.01f);

            m_MouseLastPosition = event.Mouse.Position;
        }
        break;
    case EventType::KeyPress:
        switch (event.Keyboard.Key)
        {        case ' ':
            ResetScene();
            break;
        case 't':
        case 'T':
            if (m_ProjectionType == ProjectionType::Perspective)
            {
                m_ProjectionType = ProjectionType::Orthographic;
            }
            else
            {
                m_ProjectionType = ProjectionType::Perspective;
            }
            break;
        case 'f':
        case 'F':
            switch (m_ShaderType)
            {
            case ShaderType::Wireframe:
                m_ShaderType = ShaderType::Flat;
                break;
            case ShaderType::Flat:
                m_ShaderType = ShaderType::Phong;
                break;
            case ShaderType::Phong:
                m_ShaderType = ShaderType::Wireframe;
                break;
            }
            break;
        case 'n':
        case 'N':
            m_ShowNormals = !m_ShowNormals;
            break;
        case 'g':
        case 'G':
            switch (m_Target)
            {
            case Target::ModelPosition:
                m_Target = Target::ModelRotation;
                break;
            case Target::ModelRotation:
                m_Target = Target::Light;
                break;
            case Target::Light:
                m_Target = Target::ModelPosition;
                break;
            }
            break;
        case 'c':
        case 'C':
            m_Renderer.SetFaceCulling(!m_Renderer.IsFaceCulling());
            break;
        case '1':
            m_Texture = Sampler::CreateColorSampler(ColorRGB(1.0f, 1.0f, 1.0f));
            break;
        case '2':
            m_Texture = Sampler::CreateColorSampler(ColorRGB(1.0f, 0.0f, 0.0f));
            break;
        case '3':
            m_Texture = Sampler::CreateColorSampler(ColorRGB(0.0f, 1.0f, 0.0f));
            break;
        case '4':
            m_Texture = Sampler::CreateColorSampler(ColorRGB(0.0f, 0.0f, 1.0f));
            break;
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            {
                std::string path = "Trab4JaimeADF/textures/" + std::string(1, event.Keyboard.Key) + ".bmp";
                Image image = BMP::Load(path);

                m_Texture = Sampler::CreateImageSampler(image);
            }
            break;
        }
    default:
        break;
    }
}

void PreviewPanel::Update()
{
    float cameraPositionOffset = 1.0f * Time::GetDeltaTime();

    float targetPositionOffset = 0.25f * Time::GetDeltaTime();
    float targetAngleOffset = 0.5f * Time::GetDeltaTime();

    if (Keyboard::IsKeyDown('w'))
    {
        m_Camera.SetPosition(m_Camera.GetPosition() + m_Camera.GetFront() * cameraPositionOffset);
    }

    if (Keyboard::IsKeyDown('s'))
    {
        m_Camera.SetPosition(m_Camera.GetPosition() - m_Camera.GetFront() * cameraPositionOffset);
    }

    if (Keyboard::IsKeyDown('a'))
    {
        m_Camera.SetPosition(m_Camera.GetPosition() - m_Camera.GetRight() * cameraPositionOffset);
    }

    if (Keyboard::IsKeyDown('d'))
    {
        m_Camera.SetPosition(m_Camera.GetPosition() + m_Camera.GetRight() * cameraPositionOffset);
    }
    
    if (Keyboard::IsKeyDown('j'))
    {
        switch (m_Target)
        {
        case Target::ModelPosition:
            m_ModelPosition -= m_Camera.GetRight() * targetPositionOffset;
            break;
        case Target::ModelRotation:
            m_ModelRotation.Y -= targetAngleOffset;
            break;
        case Target::Light:
            m_LightPosition -= m_Camera.GetRight() * targetPositionOffset;
            break;
        }
    }

    if (Keyboard::IsKeyDown('l'))
    {
        switch (m_Target)
        {
        case Target::ModelPosition:
            m_ModelPosition += m_Camera.GetRight() * targetPositionOffset;
            break;
        case Target::ModelRotation:
            m_ModelRotation.Y += targetAngleOffset;
            break;
        case Target::Light:
            m_LightPosition += m_Camera.GetRight() * targetPositionOffset;
            break;
        }
    }
    
    if (Keyboard::IsKeyDown('i'))
    {
        switch (m_Target)
        {
        case Target::ModelPosition:
            m_ModelPosition += m_Camera.GetFront() * targetPositionOffset;
            break;
        case Target::ModelRotation:
            m_ModelRotation.X -= targetAngleOffset;
            break;
        case Target::Light:
            m_LightPosition += m_Camera.GetFront() * targetPositionOffset;
            break;
        }
    }

    if (Keyboard::IsKeyDown('k'))
    {
        switch (m_Target)
        {
        case Target::ModelPosition:
            m_ModelPosition -= m_Camera.GetFront() * targetPositionOffset;
            break;
        case Target::ModelRotation:
            m_ModelRotation.X += targetAngleOffset;
            break;
        case Target::Light:
            m_LightPosition -= m_Camera.GetFront() * targetPositionOffset;
            break;
        }
    }

    if (Keyboard::IsKeyDown('u'))
    {
        switch (m_Target)
        {
        case Target::ModelPosition:
            m_ModelPosition.Y -= targetPositionOffset;
            break;
        case Target::ModelRotation:
            m_ModelRotation.Z += targetAngleOffset;
            break;
        case Target::Light:
            m_LightPosition.Y -= targetPositionOffset;
            break;
        }
    }

    if (Keyboard::IsKeyDown('o'))
    {
        switch (m_Target)
        {
        case Target::ModelPosition:
            m_ModelPosition.Y += targetPositionOffset;
            break;
        case Target::ModelRotation:
            m_ModelRotation.Z -= targetAngleOffset;
            break;
        case Target::Light:
            m_LightPosition.Y += targetPositionOffset;
            break;
        }
    }

    UpdateProjectionMatrix();

    m_ViewMatrix = m_Camera.GetViewMatrix();
}


void PreviewPanel::Draw()
{
    std::size_t width = static_cast<std::size_t>(GetSize().X);
    std::size_t height = static_cast<std::size_t>(GetSize().Y);

    m_Renderer.Resize(width, height);

    m_Renderer.ClearFrameBuffer();
    m_Renderer.ClearDepthBuffer();

    RenderLight();
    RenderModel();

    if (m_ShowNormals)
    {
        RenderNormals();
    }

    Graphics::DrawImage(m_Renderer.GetFrameBuffer(), GetPosition());

    DrawTarget();
}

void PreviewPanel::RenderLight()
{
    FlatShader lightShader;
    lightShader.ProjectionMatrix = m_ProjectionMatrix;
    lightShader.ViewMatrix = m_ViewMatrix;
    lightShader.ModelMatrix = Matrix4x4::Translate(m_LightPosition) * Matrix4x4::Scale(Vector3(0.1f, 0.1f, 0.1f));
    lightShader.Texture = Sampler::CreateColorSampler(ColorRGB(1.0f, 1.0f, 1.0f));

    m_Renderer.Render(m_SphereMesh, lightShader);
}

void PreviewPanel::RenderModel()
{
    Matrix4x4 translation = Matrix4x4::Translate(m_ModelPosition);
    Matrix4x4 pitchRotation = Matrix4x4::Rotate(Vector3(1.0f, 0.0f, 0.0f), m_ModelRotation.X);
    Matrix4x4 yawRotation = Matrix4x4::Rotate(Vector3(0.0f, 1.0f, 0.0f), m_ModelRotation.Y);
    Matrix4x4 rollRotation = Matrix4x4::Rotate(Vector3(0.0f, 0.0f, 1.0f), m_ModelRotation.Z);

    Matrix4x4 modelMatrix = translation * yawRotation * pitchRotation * rollRotation;

    switch (m_ShaderType)
    {
    case ShaderType::Wireframe:
        {
            WireframeShader wireframeShader;
            wireframeShader.ProjectionMatrix = m_ProjectionMatrix;
            wireframeShader.ViewMatrix = m_ViewMatrix;
            wireframeShader.ModelMatrix = modelMatrix;
            wireframeShader.Texture = m_Texture;
            m_Renderer.Render(GlobalContext::GetMesh(), wireframeShader);
        }
        break;
    case ShaderType::Flat:
        {
            FlatShader flatShader;
            flatShader.ProjectionMatrix = m_ProjectionMatrix;
            flatShader.ViewMatrix = m_ViewMatrix;
            flatShader.ModelMatrix = modelMatrix;
            flatShader.Texture = m_Texture;
            m_Renderer.Render(GlobalContext::GetMesh(), flatShader);
        }
        break;
    case ShaderType::Phong:
        {
            PhongShader phongShader;
            phongShader.ProjectionMatrix = m_ProjectionMatrix;
            phongShader.ViewMatrix = m_ViewMatrix;
            phongShader.ModelMatrix = modelMatrix;
            phongShader.LightPosition = m_LightPosition;
            phongShader.Texture = m_Texture;
            m_Renderer.Render(GlobalContext::GetMesh(), phongShader);
        }
        break;        
    }
}

void PreviewPanel::RenderNormals()
{
    const Mesh& mesh = GlobalContext::GetMesh();
    
    Matrix4x4 modelTranslation = Matrix4x4::Translate(m_ModelPosition);
    Matrix4x4 modelPitchRotation = Matrix4x4::Rotate(Vector3(1.0f, 0.0f, 0.0f), m_ModelRotation.X);
    Matrix4x4 modelYawRotation = Matrix4x4::Rotate(Vector3(0.0f, 1.0f, 0.0f), m_ModelRotation.Y);
    Matrix4x4 modelRollRotation = Matrix4x4::Rotate(Vector3(0.0f, 0.0f, 1.0f), m_ModelRotation.Z);

    Matrix4x4 modelMatrix = modelTranslation * modelYawRotation * modelPitchRotation * modelRollRotation;
    Matrix4x4 normalMatrix = modelYawRotation * modelPitchRotation * modelRollRotation;
    
    Matrix4x4 offset = Matrix4x4::Translate(Vector3(0.0f, 1.5f, 0.0f));
    Matrix4x4 scale = Matrix4x4::Scale(Vector3(0.02f, 0.02, 0.02f));
    
    FlatShader shader;
    shader.ProjectionMatrix = m_ProjectionMatrix;
    shader.ViewMatrix = m_ViewMatrix;
    shader.Texture = Sampler::CreateCoordinatesSampler();
    
    for (const auto& vertex : mesh.Vertices)
    {
        // Transform vertex position using the full model matrix
        Vector4 transformedPos = modelMatrix * Vector4(vertex.Position.X, vertex.Position.Y, vertex.Position.Z, 1.0f);
        Vector3 position = Vector3(transformedPos.X, transformedPos.Y, transformedPos.Z);
        
        // Transform normal using only the rotation part (no translation or scaling)
        Vector4 transformedNormal = normalMatrix * Vector4(vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z, 0.0f);
        Vector3 normal = Vector3(transformedNormal.X, transformedNormal.Y, transformedNormal.Z).Normalize();

        Matrix4x4 translation = Matrix4x4::Translate(position);
        Matrix4x4 rotation = Matrix4x4::Identity();

        Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
        
        if (std::abs(normal.Dot(up)) < 0.99f)
        {
            Vector3 axis = up.Cross(normal).Normalize();
            float angle = std::acos(up.Dot(normal));

            rotation = Matrix4x4::Rotate(axis, angle);
        }
        else if (normal.Dot(up) < 0.0f)
        {
            rotation = Matrix4x4::Rotate(Vector3(1.0f, 0.0f, 0.0f), MATH_PI);
        }
        
        shader.ModelMatrix = translation * rotation * scale * offset;
        
        m_Renderer.Render(m_ArrowMesh, shader);
    }
}

void PreviewPanel::DrawTarget()
{
    std::string name;

    switch (m_Target)
    {
    case Target::ModelPosition:
        name = "Model Position";
        break;
    case Target::ModelRotation:
        name = "Model Rotation";
        break;
    case Target::Light:
        name = "Light Position";
        break;
    }

    Vector2 position = GetPosition() + Vector2(10.0f, 10.0f);
    Graphics::DrawString(ColorRGB(1.0f, 1.0f, 1.0f), position, "Target: " + name);
}

void PreviewPanel::ResetScene()
{
    m_Camera.SetPosition(Vector3(0.0f, 0.0f, 2.0f));
    m_Camera.SetYaw(-MATH_PI / 2.0f);
    m_Camera.SetPitch(0.0f);

    m_ModelPosition = Vector3(0.0f, 0.0f, 0.0f);
    m_ModelRotation = Vector3(0.0f, 0.0f, 0.0f);

    m_LightPosition = Vector3(0.75f, 0.75f, 0.0f);
}

void PreviewPanel::UpdateProjectionMatrix()
{
    float aspectRatio = static_cast<float>(GetSize().X) / GetSize().Y;
    
    if (m_ProjectionType == ProjectionType::Perspective)
    {
        m_ProjectionMatrix = Matrix4x4::Perspective(MATH_PI / 4.0f, aspectRatio, 0.1f, 100.0f);
    }
    else
    {
        float orthoSize = 1.0f;
        float left = -orthoSize * aspectRatio;
        float right = orthoSize * aspectRatio;
        float bottom = -orthoSize;
        float top = orthoSize;
        m_ProjectionMatrix = Matrix4x4::Ortographic(left, right, bottom, top, 0.1f, 100.0f);
    }
}