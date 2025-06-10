#include <sstream>
#include <iomanip>
#include <iostream>

#include "EditorPanel.h"

void GenerateSpring(PolyLine3D& polyline, int segments, float radius, float height, float frequency)
{
    polyline.Clear();

    for (int i = 0; i <= segments; ++i)
    {
        float t = static_cast<float>(i) / static_cast<float>(segments - 1);
        float angle = t * MATH_TAU * frequency;
        float x = radius * std::cos(angle);
        float y = height * t - height / 2.0f;
        float z = radius * std::sin(angle);

        polyline.InsertPoint(Vector3(x, y, z));
    }
}

const float EditorPanel::s_PointRadius = 6.0f;
const float EditorPanel::s_LocalGridSpacing = 0.15f;

EditorPanel::EditorPanel()
{
    m_ModelType = ModelType::Revolution;

    m_BezierDegree = 3;
    m_BezierClosed = true;

    m_CurveResolution = 16;
    m_ExtrudeResolution = 16;

    m_SpringRadius = 1.0f;
    m_SpringHeight = 1.0f; 
    m_SpringFrequency = 2.0f;

    m_SelectedPointIndex = -1;

    m_ControlPoints = {
        Vector2(0.60, -0.15),
        Vector2(0.75, -0.15),
        Vector2(0.75, 0.15),
        Vector2(0.60, 0.15),
        Vector2(0.45, 0.15),
        Vector2(0.45, -0.15),
    };

    SetSize(Vector2(Window::GetWidth() / 2.0f, Window::GetHeight()));
    SetPosition(Vector2());
}

void EditorPanel::Process(const Event& event)
{
    switch (event.Type)
    {
    case EventType::WindowResize:
        SetSize(Vector2(event.Window.Width / 2.0f, event.Window.Height));
        break;
    case EventType::MouseButtonPress:
        if (IsPointInside(event.Mouse.Position))
        {
            int hoveringIndex = LocatePointAt(event.Mouse.Position);

            switch (event.Mouse.Button)
            {
            case MOUSE_BUTTON_LEFT:
                if (hoveringIndex != -1)
                {
                    SelectPoint(hoveringIndex);
                }
                else
                {
                    CreatePoint(event.Mouse.Position);
                }
                break;
            case MOUSE_BUTTON_RIGHT:
                if (hoveringIndex != -1)
                {
                    DeletePoint(hoveringIndex);
                }
                break;
            }
        }
        break;
    case EventType::MouseButtonRelease:
        if (event.Mouse.Button == MOUSE_BUTTON_LEFT)
        {
            DeselectPoint();
        }
        break;
    case EventType::MouseMove:
        if (IsPointInside(event.Mouse.Position) && m_SelectedPointIndex != -1)
        {
            MovePoint(m_SelectedPointIndex, event.Mouse.Position);
        }
        break;
    case EventType::KeyPress:
        switch (event.Keyboard.Key)
        {
        case 'v':
            m_BezierDegree = m_BezierDegree + 1;
            break;
        case 'V':
            m_BezierDegree = std::max(1, m_BezierDegree - 1);
            break;
        case 'c':
        case 'C':
            m_BezierClosed = !m_BezierClosed;
            break;
        case 'z':
            m_CurveResolution = m_CurveResolution + 1;
            break;
        case 'Z':
            m_CurveResolution = std::max(1, m_CurveResolution - 1);
            break;
        case 'x':
            m_ExtrudeResolution = m_ExtrudeResolution + 1;
            break;
        case 'X':
            m_ExtrudeResolution = std::max(1, m_ExtrudeResolution - 1);
            break;
        case 'b':
            m_SpringRadius += 0.01f;
            break;
        case 'B':
            m_SpringRadius = std::max(0.01f, m_SpringRadius - 0.01f);
            break;
        case 'n':
            m_SpringHeight += 0.01f;
            break;
        case 'N':
            m_SpringHeight = std::max(0.01f, m_SpringHeight - 0.01f);
            break;
        case 'm':
            m_SpringFrequency += 0.1f;
            break;
        case 'M':
            m_SpringFrequency = std::max(0.1f, m_SpringFrequency - 0.1f);
            break;
        case 'g':
        case 'G':
            if (m_ModelType == ModelType::Revolution)
            {
                m_ModelType = ModelType::SpringSweep;
            }
            else
            {
                m_ModelType = ModelType::Revolution;
            }
            break;
        case 'y':
        case 'Y':
            for (const Vector2& point : m_ControlPoints)
            {
                std::cout << "Vector2(" << std::fixed << std::setprecision(2) << point.X << ", " << point.Y << ")," << std::endl;
            }
            break;
        }
    default:
        break;
    }
}

void EditorPanel::Update()
{
    UpdatePoints();
    RegenerateCurve();
    RegenerateModel();
}

void EditorPanel::Draw()
{
    Graphics::FillRectangle(0x1E1E1E, GetPosition(), GetSize());
    
    DrawGrid();
    DrawBezierPolygon();
    DrawCurve();
    DrawControlPoints();
    DrawInformation();
}

void EditorPanel::DrawGrid()
{
    Vector2 localBottomLeft = ConvertToLocalCoordinates(GetPosition());
    Vector2 localTopRight = ConvertToLocalCoordinates(GetPosition() + GetSize());

    float minimumX = localBottomLeft.X;
    float minimumY = localBottomLeft.Y;

    float maximumX = localTopRight.X;
    float maximumY = localTopRight.Y;

    float startX = std::floor(minimumX / s_LocalGridSpacing) * s_LocalGridSpacing;
    float startY = std::floor(minimumY / s_LocalGridSpacing) * s_LocalGridSpacing;

    for (float x = startX; x <= maximumX; x += s_LocalGridSpacing)
    {
        Vector2 start(x, minimumY);
        Vector2 end(x, maximumY);

        m_TemporaryPolyLine.Clear();
        m_TemporaryPolyLine.InsertPoint(ConvertToScreenCoordinates(start).Round());
        m_TemporaryPolyLine.InsertPoint(ConvertToScreenCoordinates(end).Round());

        Graphics::StrokeLine(0x2E2E2E, m_TemporaryPolyLine, 2.0f);
    }

    for (float y = startY; y <= maximumY; y += s_LocalGridSpacing)
    {
        Vector2 start(minimumX, y);
        Vector2 end(maximumX, y);

        m_TemporaryPolyLine.Clear();

        m_TemporaryPolyLine.InsertPoint(ConvertToScreenCoordinates(start).Round());
        m_TemporaryPolyLine.InsertPoint(ConvertToScreenCoordinates(end).Round());

        Graphics::StrokeLine(0x2E2E2E, m_TemporaryPolyLine, 2.0f);
    }

    m_TemporaryPolyLine.Clear();
    m_TemporaryPolyLine.InsertPoint(ConvertToScreenCoordinates(Vector2(startX, 0.0)).Round());
    m_TemporaryPolyLine.InsertPoint(ConvertToScreenCoordinates(Vector2(maximumX, 0.0)).Round());

    Graphics::StrokeLine(0x3A3A5A, m_TemporaryPolyLine, 2.0f);

    m_TemporaryPolyLine.Clear();
    m_TemporaryPolyLine.InsertPoint(ConvertToScreenCoordinates(Vector2(0.0, startY)).Round());
    m_TemporaryPolyLine.InsertPoint(ConvertToScreenCoordinates(Vector2(0.0, maximumY)).Round());

    Graphics::StrokeLine(0x3A3A5A, m_TemporaryPolyLine, 2.0f);
}

void EditorPanel::DrawBezierPolygon()
{
    m_TemporaryPolyLine.Clear();
    
    for (const Vector2& point : m_BezierPoints)
    {
        m_TemporaryPolyLine.InsertPoint(ConvertToScreenCoordinates(point).Round());
    }

    Graphics::StrokeLine(0x555555, m_TemporaryPolyLine, 2.0f);
}

void EditorPanel::DrawControlPoints()
{
    for (std::size_t i = 0; i < m_ControlPoints.size(); ++i)
    {
        const Vector2& point = m_ControlPoints[i];

        Vector2 screenPoint = ConvertToScreenCoordinates(point).Round();

        Graphics::FillCircle(0xFFFFFF, screenPoint, s_PointRadius);
        Graphics::DrawString(0xFFFFFF, screenPoint + Vector2(10, -10), std::to_string(i));
    }
}

void EditorPanel::DrawCurve()
{
    m_TemporaryPolyLine.Clear();

    for (std::size_t i = 0; i < m_BezierPolyLine.GetSize(); ++i)
    {
        Vector2 point = m_BezierPolyLine.GetPoint(i);
        m_TemporaryPolyLine.InsertPoint(ConvertToScreenCoordinates(point).Round());
    }

    Graphics::StrokeLine(0x3ECCFD, m_TemporaryPolyLine, 2.0f);
}

void EditorPanel::DrawInformation()
{
    std::stringstream curveStream;
    curveStream << "Degree: " << m_BezierDegree;
    curveStream << "; Curve Res.: " << m_CurveResolution;
    curveStream << "; Extrude Res.: " << m_ExtrudeResolution;
    curveStream << "; Closed: " << (m_BezierClosed ? "Yes" : "No");

    std::stringstream modelStream;
    if (m_ModelType == ModelType::Revolution)
    {
        modelStream << "Model: Revolution";
    }
    else
    {
        modelStream << "Model: Spring";
        modelStream << "; Radius: " << std::fixed << std::setprecision(2) << m_SpringRadius;
        modelStream << "; Height: " << std::fixed << std::setprecision(2) << m_SpringHeight;
        modelStream << "; Frequency: " << std::fixed << std::setprecision(2) << m_SpringFrequency;
    }

    Graphics::DrawString(0xFFFFFF, Vector2(16, 40), curveStream.str());
    Graphics::DrawString(0xFFFFFF, Vector2(16, 16), modelStream.str());
}

int EditorPanel::LocatePointAt(const Vector2& screenPosition) const
{
    for (std::size_t i = 0; i < m_ControlPoints.size(); ++i)
    {
        Vector2 screenPoint = ConvertToScreenCoordinates(m_ControlPoints[i]);

        if (IsPointInCircle(screenPosition, screenPoint, s_PointRadius))
        {
            return static_cast<int>(i);
        }
    }

    return -1;
}

void EditorPanel::SelectPoint(int index)
{
    m_SelectedPointIndex = index;
}

void EditorPanel::DeselectPoint()
{
    m_SelectedPointIndex = -1;
}

void EditorPanel::CreatePoint(const Vector2& screenPosition)
{
    m_ControlPoints.emplace_back(ConvertToLocalCoordinates(screenPosition));
}

void EditorPanel::DeletePoint(int index)
{
    m_ControlPoints.erase(m_ControlPoints.begin() + index);

    if (m_SelectedPointIndex == index)
    {
        DeselectPoint();
    }
}

void EditorPanel::MovePoint(int index, const Vector2& screenPosition)
{
    m_ControlPoints[index] = ConvertToLocalCoordinates(screenPosition);
}

void EditorPanel::UpdatePoints()
{
    m_BezierPoints.clear();

    if (m_ControlPoints.empty())
    {
        return;
    }

    if (m_BezierClosed)
    {
        if (m_ControlPoints.size() < static_cast<std::size_t>(m_BezierDegree))
        {
            return;
        }

        std::size_t patches = m_ControlPoints.size() / m_BezierDegree;
        std::size_t n = patches * m_BezierDegree;

        for (std::size_t i = 0; i <= n; ++i)
        {
            m_BezierPoints.push_back(m_ControlPoints[i % n]);
        }
    }
    else
    {
        if (m_ControlPoints.size() < static_cast<std::size_t>(m_BezierDegree + 1))
        {
            return;
        }

        std::size_t patches = (m_ControlPoints.size() - 1) / m_BezierDegree;
        std::size_t n = patches * m_BezierDegree + 1;

        for (std::size_t i = 0; i < n; ++i)
        {
            m_BezierPoints.push_back(m_ControlPoints[i]);
        }
    }
}

void EditorPanel::RegenerateCurve()
{
    m_BezierPolyLine.Clear();

    int patches = (static_cast<int>(m_BezierPoints.size()) - 1) / m_BezierDegree;

    if (patches == 0)
    {
        return;
    }

    float step = patches / static_cast<float>(m_CurveResolution);

    for (int i = 0; i <= m_CurveResolution; ++i)
    {
        float v = i * step;
        int patch = static_cast<int>(v);

        float t = std::min(v - patch, 1.0f);

        auto start = m_BezierPoints.begin() + patch * m_BezierDegree;
        auto end = start + m_BezierDegree + 1;

        Vector2 point = m_Bezier.Evaluate(start, end, t);

        m_BezierPolyLine.InsertPoint(point);
    }
}

void EditorPanel::RegenerateModel()
{
    m_PathPolyLine.Clear();

    if (m_ModelType == ModelType::Revolution)
    {
        Mesh::GenerateRevolution(GlobalContext::GetMesh(), m_BezierPolyLine, m_ExtrudeResolution);
    }
    else
    {
        GenerateSpring(m_PathPolyLine, m_ExtrudeResolution, m_SpringRadius, m_SpringHeight, m_SpringFrequency);
        Mesh::GenerateSweep(GlobalContext::GetMesh(), m_BezierPolyLine, m_PathPolyLine);
    }
}

float EditorPanel::ComputeScale() const
{
    return GetSize().X / 2.0f;
}

Vector2 EditorPanel::ConvertToLocalCoordinates(const Vector2& point) const
{
    Vector2 center = GetPosition() + GetSize() / 2.0f;

    return (point - center) / ComputeScale();
}

Vector2 EditorPanel::ConvertToScreenCoordinates(const Vector2& point) const
{
    Vector2 center = GetPosition() + GetSize() / 2.0f;

    return center + point * ComputeScale();
}
