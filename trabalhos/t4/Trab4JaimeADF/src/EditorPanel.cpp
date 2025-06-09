#include "EditorPanel.h"

const float EditorPanel::s_PointRadius = 5.0f;
const float EditorPanel::s_LocalGridSpacing = 0.075f;

EditorPanel::EditorPanel()
{
    m_SelectedPointIndex = -1;
    m_BezierDegree = 3;
    m_BezierSegments = 10;
    m_BezierArcs = 8;

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
        case 'q':
            IncreaseDegree();
            break;
        case 'Q':
            DecreaseDegree();
            break;
        case 'e':
            IncreaseSamples();
            break;
        case 'E':
            DecreaseSamples();
            break;
        case 'r':
            IncreaseArcs();
            break;
        case 'R':
            DecreaseArcs();
            break;
        }
    default:
        break;
    }
}

void EditorPanel::Update()
{
    RegenerateCurve();
    Mesh::GenerateRevolution(GlobalContext::GetMesh(), m_CurvePolyLine, m_BezierArcs);
}

void EditorPanel::Draw()
{
    Graphics::FillRectangle(0x1E1E1E, GetPosition(), GetSize());
    
    DrawGrid();
    DrawControlPolygon();
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

void EditorPanel::DrawControlPolygon()
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
    for (const Vector2& point : m_BezierPoints)
    {
        Vector2 screenPoint = ConvertToScreenCoordinates(point);
        Graphics::FillCircle(0xFFFFFF, screenPoint, s_PointRadius);
    }
}

void EditorPanel::DrawCurve()
{
    m_TemporaryPolyLine.Clear();

    for (std::size_t i = 0; i < m_CurvePolyLine.GetSize(); ++i)
    {
        Vector2 point = m_CurvePolyLine.GetPoint(i);
        m_TemporaryPolyLine.InsertPoint(ConvertToScreenCoordinates(point).Round());
    }

    Graphics::StrokeLine(0x3ECCFD, m_TemporaryPolyLine, 2.0f);
}

void EditorPanel::DrawInformation()
{
    std::string information;
    information += "Degree: " + std::to_string(m_BezierDegree);
    information += "; ";
    information += "Segments: " + std::to_string(m_BezierSegments);
    information += "; ";
    information += "Arcs: " + std::to_string(m_BezierArcs);

    Graphics::DrawString(0xFFFFFF, Vector2(16, 16), information);
}

int EditorPanel::LocatePointAt(const Vector2& screenPosition) const
{
    for (std::size_t i = 0; i < m_BezierPoints.size(); ++i)
    {
        Vector2 screenPoint = ConvertToScreenCoordinates(m_BezierPoints[i]);

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
    m_BezierPoints.emplace_back(ConvertToLocalCoordinates(screenPosition));
}

void EditorPanel::DeletePoint(int index)
{
    m_BezierPoints.erase(m_BezierPoints.begin() + index);

    if (m_SelectedPointIndex == index)
    {
        DeselectPoint();
    }
}

void EditorPanel::MovePoint(int index, const Vector2& screenPosition)
{
    m_BezierPoints[index] = ConvertToLocalCoordinates(screenPosition);
}

void EditorPanel::IncreaseDegree()
{
    m_BezierDegree++;
}

void EditorPanel::DecreaseDegree()
{
    if (m_BezierDegree > 1)
    {
        m_BezierDegree--;
    }
}

void EditorPanel::IncreaseSamples()
{
    m_BezierSegments++;
}

void EditorPanel::DecreaseSamples()
{
    if (m_BezierSegments > 1)
    {
        m_BezierSegments--;
    }
}

void EditorPanel::IncreaseArcs()
{
    m_BezierArcs++;
}

void EditorPanel::DecreaseArcs()
{
    if (m_BezierArcs > 3)
    {
        m_BezierArcs--;
    }
}

void EditorPanel::RegenerateCurve()
{
    m_CurvePolyLine.Clear();

    int patches = (static_cast<int>(m_BezierPoints.size()) - 1) / m_BezierDegree;

    if (patches == 0)
    {
        return;
    }

    float step = patches / static_cast<float>(m_BezierSegments);

    for (int i = 0; i <= m_BezierSegments; ++i)
    {
        float v = i * step;
        int patch = static_cast<int>(v);

        float t = v - patch;

        auto start = m_BezierPoints.begin() + patch * m_BezierDegree;
        auto end = start + m_BezierDegree + 1;

        Vector2 point = m_Bezier.Evaluate(start, end, t);

        m_CurvePolyLine.InsertPoint(point);
    }
}

float EditorPanel::ComputeScale() const
{
    return GetSize().X;
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
