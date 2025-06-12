#include <iostream>
#include <sstream>
#include <iomanip>

#include "PreviewPanel.h"
#include "WireframeShader.h"
#include "FlatShader.h"
#include "WireframeShader.h"
#include "PhongShader.h"
#include "GlobalContext.h"
#include "BMP.h"
#include "Material.h"

PreviewPanel::PreviewPanel() : Panel()
{
    Mesh::GenerateSphere(m_SphereMesh, 32, 32);
    Mesh::GenerateArrow(m_ArrowMesh, 8);
    m_ProjectionType = ProjectionType::Perspective;
    m_ShaderType = ShaderType::Wireframe;
    m_NormalMode = NormalMode::Hidden;
    m_MaterialType = MaterialType::Rubber;
    m_Target = Target::ModelPosition;
    m_MouseDragging = false;

    ResetScene();

    SetSize(Vector2(Window::GetWidth() / 2.0f, Window::GetHeight()));
    SetPosition(Vector2(Window::GetWidth() / 2.0f + 0.5f, 0.5f));
}

void PreviewPanel::Process(const Event &event)
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

            if (m_ProjectionType == ProjectionType::Perspective)
            {
                m_Camera.SetYaw(m_Camera.GetYaw() + delta.X * 0.01f);
                m_Camera.SetPitch(m_Camera.GetPitch() + delta.Y * 0.01f);
            }
            else
            {
                float panSpeed = 0.01f * m_Camera.GetZoom();
                Vector3 currentPos = m_Camera.GetPosition();
                currentPos.X += delta.X * panSpeed;
                currentPos.Y += delta.Y * panSpeed;
                m_Camera.SetPosition(currentPos);
            }

            m_MouseLastPosition = event.Mouse.Position;
        }
        break;
    case EventType::MouseScroll:
        if (IsPointInside(event.Mouse.Position))
        {
            float zoomFactor = -0.1f;

            if (m_ProjectionType == ProjectionType::Perspective)
            {
                float currentZoom = m_Camera.GetZoom();
                currentZoom -= event.Mouse.Scroll.Y * zoomFactor;
                m_Camera.SetZoom(currentZoom);
            }
            else
            {
                float currentZoom = m_Camera.GetZoom();
                currentZoom += event.Mouse.Scroll.Y * zoomFactor;
                m_Camera.SetZoom(currentZoom);
            }
        }
        break;
    case EventType::KeyPress:
        switch (event.Keyboard.Key)
        {
        case ' ':
            ResetScene();
            break;
        case 'q':
        case 'Q':
            if (m_ProjectionType == ProjectionType::Perspective)
            {
                m_ProjectionType = ProjectionType::Orthographic;
            }
            else
            {
                m_ProjectionType = ProjectionType::Perspective;
            }
            ResetCamera();
            break;
        case 'r':
        case 'R':
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
        case 'f':
        case 'F':
            switch (m_NormalMode)
            {
            case NormalMode::Hidden:
                m_NormalMode = NormalMode::Vertex;
                break;
            case NormalMode::Vertex:
                m_NormalMode = NormalMode::Face;
                break;
            case NormalMode::Face:
                m_NormalMode = NormalMode::Hidden;
                break;
            }
            break;
        case 'p':
        case 'P':
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
            break;        case 'e':
        case 'E':
            m_Renderer.SetFaceCulling(!m_Renderer.IsFaceCulling());
            break;        case 'y':
        case 'Y':
            switch (m_MaterialType)
            {
            case MaterialType::Rubber:
                m_MaterialType = MaterialType::Plastic;
                break;
            case MaterialType::Plastic:
                m_MaterialType = MaterialType::Metal;
                break;
            case MaterialType::Metal:
                m_MaterialType = MaterialType::Rubber;
                break;
            }
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

    if (m_ProjectionType == ProjectionType::Perspective)
    {
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
    }
    else
    {
        Vector3 cameraRight = Vector3(1.0f, 0.0f, 0.0f);
        Vector3 cameraUp = Vector3(0.0f, 1.0f, 0.0f);

        if (Keyboard::IsKeyDown('w'))
        {
            m_Camera.SetPosition(m_Camera.GetPosition() + cameraUp * cameraPositionOffset);
        }

        if (Keyboard::IsKeyDown('s'))
        {
            m_Camera.SetPosition(m_Camera.GetPosition() - cameraUp * cameraPositionOffset);
        }

        if (Keyboard::IsKeyDown('a'))
        {
            m_Camera.SetPosition(m_Camera.GetPosition() - cameraRight * cameraPositionOffset);
        }

        if (Keyboard::IsKeyDown('d'))
        {
            m_Camera.SetPosition(m_Camera.GetPosition() + cameraRight * cameraPositionOffset);
        }
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
    UpdateViewMatrix();
    UpdateModelMatrix();
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

    switch (m_NormalMode)
    {
    case NormalMode::Vertex:
        RenderVertexNormals();
        break;
    case NormalMode::Face:
        RenderFaceNormals();
        break;
    case NormalMode::Hidden:
        break;
    }

    Graphics::DrawImage(m_Renderer.GetFrameBuffer(), GetPosition());

    DrawInformation();
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
    switch (m_ShaderType)
    {
    case ShaderType::Wireframe:
    {
        WireframeShader wireframeShader;
        wireframeShader.ProjectionMatrix = m_ProjectionMatrix;
        wireframeShader.ViewMatrix = m_ViewMatrix;
        wireframeShader.ModelMatrix = m_ModelMatrix;
        wireframeShader.Texture = m_Texture;
        m_Renderer.Render(GlobalContext::GetMesh(), wireframeShader);
    }
    break;
    case ShaderType::Flat:
    {
        FlatShader flatShader;
        flatShader.ProjectionMatrix = m_ProjectionMatrix;
        flatShader.ViewMatrix = m_ViewMatrix;
        flatShader.ModelMatrix = m_ModelMatrix;
        flatShader.Texture = m_Texture;
        m_Renderer.Render(GlobalContext::GetMesh(), flatShader);
    }
    break;    case ShaderType::Phong:
    {
        PhongShader phongShader;
        phongShader.ProjectionMatrix = m_ProjectionMatrix;
        phongShader.ViewMatrix = m_ViewMatrix;
        phongShader.ModelMatrix = m_ModelMatrix;
        phongShader.LightPosition = m_LightPosition;
        phongShader.CameraPosition = m_Camera.GetPosition();
        phongShader.Texture = m_Texture;

        switch (m_MaterialType)
        {
        case MaterialType::Rubber:
            phongShader.Material = Material::CreateRubber();
            break;
        case MaterialType::Plastic:
            phongShader.Material = Material::CreatePlastic();
            break;
        case MaterialType::Metal:
            phongShader.Material = Material::CreateMetal();
            break;
        }
        
        m_Renderer.Render(GlobalContext::GetMesh(), phongShader);
    }
    break;
    }
}

void PreviewPanel::RenderVertexNormals()
{
    const Mesh &mesh = GlobalContext::GetMesh();

    Matrix4x4 offset = Matrix4x4::Translate(Vector3(0.0f, 1.5f, 0.0f));
    Matrix4x4 scale = Matrix4x4::Scale(Vector3(0.01f, 0.01, 0.01f));

    FlatShader shader;
    shader.ProjectionMatrix = m_ProjectionMatrix;
    shader.ViewMatrix = m_ViewMatrix;
    shader.Texture = Sampler::CreateCoordinatesSampler();

    for (const auto &vertex : mesh.Vertices)
    {
        Vector4 transformedPos = m_ModelMatrix * Vector4(vertex.Position.X, vertex.Position.Y, vertex.Position.Z, 1.0f);
        Vector3 position = Vector3(transformedPos.X, transformedPos.Y, transformedPos.Z);

        Vector4 transformedNormal = m_ModelMatrix * Vector4(vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z, 0.0f);
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

void PreviewPanel::RenderFaceNormals()
{
    const Mesh &mesh = GlobalContext::GetMesh();
    
    Matrix4x4 offset = Matrix4x4::Translate(Vector3(0.0f, 1.5f, 0.0f));
    Matrix4x4 scale = Matrix4x4::Scale(Vector3(0.01f, 0.01, 0.01f));

    FlatShader shader;
    shader.ProjectionMatrix = m_ProjectionMatrix;
    shader.ViewMatrix = m_ViewMatrix;
    shader.Texture = Sampler::CreateCoordinatesSampler();

    for (std::size_t i = 0; i < mesh.Indices.size(); i += 3)
    {
        const Vertex& v0 = mesh.Vertices[mesh.Indices[i]];
        const Vertex& v1 = mesh.Vertices[mesh.Indices[i + 1]];
        const Vertex& v2 = mesh.Vertices[mesh.Indices[i + 2]];

        Vector3 worldPos0 = (m_ModelMatrix * v0.Position).ToCartesianPosition();
        Vector3 worldPos1 = (m_ModelMatrix * v1.Position).ToCartesianPosition();
        Vector3 worldPos2 = (m_ModelMatrix * v2.Position).ToCartesianPosition();

        Vector3 faceCenter = (worldPos0 + worldPos1 + worldPos2) / 3.0f;

        Vector3 edge1 = worldPos1 - worldPos0;
        Vector3 edge2 = worldPos2 - worldPos0;
        Vector3 faceNormal = edge1.Cross(edge2).Normalize();

        Matrix4x4 translation = Matrix4x4::Translate(faceCenter);
        Matrix4x4 rotation = Matrix4x4::Identity();

        Vector3 up = Vector3(0.0f, 1.0f, 0.0f);

        if (std::abs(faceNormal.Dot(up)) < 0.99f)
        {
            Vector3 axis = up.Cross(faceNormal).Normalize();
            float angle = std::acos(up.Dot(faceNormal));

            rotation = Matrix4x4::Rotate(axis, angle);
        }
        else if (faceNormal.Dot(up) < 0.0f)
        {
            rotation = Matrix4x4::Rotate(Vector3(1.0f, 0.0f, 0.0f), MATH_PI);
        }

        shader.ModelMatrix = translation * rotation * scale * offset;

        m_Renderer.Render(m_ArrowMesh, shader);
    }
}

void PreviewPanel::DrawInformation()
{
    std::stringstream optionsLine1;
    std::stringstream optionsLine2;
    std::stringstream objectLine;

    objectLine << "Target: ";

    switch (m_Target)
    {
    case Target::ModelPosition:
        objectLine << "Model Position";
        break;
    case Target::ModelRotation:
        objectLine << "Model Rotation";
        break;
    case Target::Light:
        objectLine << "Light Position";
        break;
    }

    optionsLine2 << "Projection: ";

    switch (m_ProjectionType)
    {
    case ProjectionType::Perspective:
        optionsLine2 << "Perspective";
        break;
    case ProjectionType::Orthographic:
        optionsLine2 << "Orthographic";
        break;
    }

    optionsLine2 << "; Face Culling: ";
    optionsLine2 << (m_Renderer.IsFaceCulling() ? "yes" : "no");

    optionsLine2 << "; Normals: ";

    switch (m_NormalMode)
    {
    case NormalMode::Hidden:
        optionsLine2 << "Hidden";
        break;
    case NormalMode::Vertex:
        optionsLine2 << "Vertex";
        break;
    case NormalMode::Face:
        optionsLine2 << "Face";
        break;
    }

    optionsLine1 << "Shader: ";

    switch (m_ShaderType)
    {
    case ShaderType::Wireframe:
        optionsLine1 << "Wireframe";
        break;
    case ShaderType::Flat:
        optionsLine1 << "Flat";
        break;
    case ShaderType::Phong:
        optionsLine1 << "Phong";
        break;
    }

    optionsLine1 << "; Material: ";

    switch (m_MaterialType)
    {
    case MaterialType::Rubber:
        optionsLine1 << "Rubber";
        break;
    case MaterialType::Plastic:
        optionsLine1 << "Plastic";
        break;
    case MaterialType::Metal:
        optionsLine1 << "Metal";
        break;
    }

    const Mesh &mesh = GlobalContext::GetMesh();

    optionsLine1 << "; Vertices: " << mesh.Vertices.size();
    optionsLine1 << "; Faces: " << mesh.Indices.size() / 3;

    const Vector2& position = GetPosition();

    Graphics::DrawString(ColorRGB(1.0f, 1.0f, 1.0f), position + Vector2(16.0f, 16.0f), objectLine.str());
    Graphics::DrawString(ColorRGB(1.0f, 1.0f, 1.0f), position + Vector2(16.0f, 40.0f), optionsLine1.str());
    Graphics::DrawString(ColorRGB(1.0f, 1.0f, 1.0f), position + Vector2(16.0f, 64.0f), optionsLine2.str());
}

void PreviewPanel::ResetScene()
{
    ResetCamera();

    m_ModelPosition = Vector3(0.0f, 0.0f, 0.0f);
    m_ModelRotation = Vector3(0.0f, 0.0f, 0.0f);

    m_LightPosition = Vector3(0.75f, 0.75f, 0.0f);

    m_Camera.SetZoom(1.0f);
}

void PreviewPanel::ResetCamera()
{
    if (m_ProjectionType == ProjectionType::Perspective)
    {
        m_Camera.SetPosition(Vector3(0.0f, 0.0f, 3.0f));
        m_Camera.SetYaw(-MATH_PI / 2.0f);
        m_Camera.SetPitch(0.0f);
    }
    else
    {
        m_Camera.SetPosition(Vector3(0.0f, 0.0f, 5.0f));
        m_Camera.SetYaw(-MATH_PI / 2.0f);
        m_Camera.SetPitch(0.0f);
    }
}

void PreviewPanel::UpdateProjectionMatrix()
{
    float aspectRatio = static_cast<float>(GetSize().X) / GetSize().Y;

    if (m_ProjectionType == ProjectionType::Perspective)
    {
        float baseFov = MATH_PI / 4.0f;
        float fov = baseFov / m_Camera.GetZoom();
        m_ProjectionMatrix = Matrix4x4::Perspective(fov, aspectRatio, 0.1f, 100.0f);
    }
    else
    {
        float orthoSize = m_Camera.GetZoom();
        float left = -orthoSize * aspectRatio;
        float right = orthoSize * aspectRatio;
        float bottom = -orthoSize;
        float top = orthoSize;
        m_ProjectionMatrix = Matrix4x4::Ortographic(left, right, bottom, top, 0.1f, 100.0f);
    }
}

void PreviewPanel::UpdateViewMatrix()
{
    m_ViewMatrix = m_Camera.GetViewMatrix();
}

void PreviewPanel::UpdateModelMatrix()
{
    Matrix4x4 translation = Matrix4x4::Translate(m_ModelPosition);
    Matrix4x4 pitchRotation = Matrix4x4::Rotate(Vector3(1.0f, 0.0f, 0.0f), m_ModelRotation.X);
    Matrix4x4 yawRotation = Matrix4x4::Rotate(Vector3(0.0f, 1.0f, 0.0f), m_ModelRotation.Y);
    Matrix4x4 rollRotation = Matrix4x4::Rotate(Vector3(0.0f, 0.0f, 1.0f), m_ModelRotation.Z);

    m_ModelMatrix = translation * yawRotation * pitchRotation * rollRotation;
}