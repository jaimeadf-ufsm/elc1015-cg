#include <limits>

#include "TrackEditor.h"
#include "Scene.h"
#include "Tesselator.h"
#include "Bezier.h"

#include "PrimitiveGraphic.h"
#include "Geometry.h"
#include "Interpolation.h"
#include "PathTransformer.h"

static const float s_PointRadius = 6.0f;
static const float s_SplitRadius = 3.0f;

// P0, C1_0, C2_0, P1, C1_1, C2_1, P2, C1_2, C2_2

size_t GetP0Index(size_t segmentIndex)
{
    return segmentIndex + 1;
}

size_t GetP1Index(size_t segmentIndex)
{
    return segmentIndex + 3;
}

TrackEditor::TrackEditor(std::reference_wrapper<Scene> scene) :
    GameObject(scene),
    m_Preview(true),
    m_Width(100.0),
    m_SelectedIndex(-1)
{

}

void TrackEditor::Initialize()
{
}

void TrackEditor::Update(float deltaTime)
{
    Scene& scene = GetScene();
    Mouse& mouse = scene.GetMouse();
    GameState& state = scene.GetGame().GetState();

    std::shared_ptr<Track> selectedTrack = state.GetSelectedTrack();

    if (selectedTrack != m_Track)
    {
        m_Track = selectedTrack;
        m_Points = m_Track->GetPoints();
        m_Width = m_Track->GetWidth();
    }

    m_Track->SetPoints(m_Points);
    m_Track->SetWidth(m_Width);

    if (m_Track != nullptr)
    {
        m_ClosestSegmentIndex = FindClosestSegmentToPoint(mouse.GetPosition(), m_ClosestSegmentT);

        if (m_ClosestSegmentIndex != -1)
        {
            m_ClosestSegmentPoint = EvaluateSegment(m_ClosestSegmentIndex, m_ClosestSegmentT);
        }
    }
}

void TrackEditor::Draw(DrawingContext& context)
{
    Scene& scene = GetScene();
    Mouse& mouse = scene.GetMouse();
    Viewport& viewport = scene.GetViewport();

    if (m_Track == nullptr)
    {
        return;
    }

    const Path& path = m_Track->ComputeCurve();

    if (m_Preview)
    {
        context.Color(0x39C272);
        context.FillRectangle(Vector2(0.0f, 0.0f), Vector2(viewport.GetWidth(), viewport.GetHeight()));

        m_Track->ComputeGraphic().Draw(context);
    }

    Tesselator::Stroke(m_TemporaryMesh, path, 2.0f);

    context.Color(0x0C8CE9);
    PrimitiveGraphic::FillMesh(context, m_TemporaryMesh);

    context.Color(ColorRGB::Red);

    PathTransformer::Offset(m_TemporaryPath, m_Track->ComputeCurve(), m_Track->GetWidth() / 2.0f);
    Tesselator::Stroke(m_TemporaryMesh, m_TemporaryPath, 2.0f);
    PrimitiveGraphic::FillMesh(context, m_TemporaryMesh);

    context.Color(ColorRGB::Red);

    PathTransformer::Offset(m_TemporaryPath, m_Track->ComputeCurve(), -m_Track->GetWidth() / 2.0f);
    Tesselator::Stroke(m_TemporaryMesh, m_TemporaryPath, 2.0f);
    PrimitiveGraphic::FillMesh(context, m_TemporaryMesh);

    for (size_t segmentIndex = 1; segmentIndex <= GetSegmentCount(); segmentIndex++)
    {
        Vector2 c1 = GetC1(segmentIndex - 1);
        Vector2 c0 = GetC0(segmentIndex);

        m_TemporaryPath.Clear();
        m_TemporaryPath.MoveTo(c1);
        m_TemporaryPath.LineTo(c0);
        
        Tesselator::Stroke(m_TemporaryMesh, m_TemporaryPath, 2.0f);

        context.Color(0xA8A8A8);
        PrimitiveGraphic::FillMesh(context, m_TemporaryMesh);
    }

    for (const Vector2& point : m_Points)
    {
        if (IsPointInCircle(mouse.GetPosition(), point, s_PointRadius))
        {
            context.Color(ColorRGB(0xABC9E6));
        }
        else
        {
            context.Color(ColorRGB(0x0C8CE9));
        }

        PrimitiveGraphic::FillCircle(context, point, s_PointRadius);

        context.Color(ColorRGB(0xFFFFFF));
        PrimitiveGraphic::FillCircle(context, point, s_PointRadius - 2.0f);
    }

    if (LocatePointAt(mouse.GetPosition()) == -1)
    {
        if (IsPointInCircle(mouse.GetPosition(), m_ClosestSegmentPoint, s_SplitRadius))
        {
            context.Color(ColorRGB(0x0C8CE9));
            PrimitiveGraphic::FillCircle(context, m_ClosestSegmentPoint, s_PointRadius);

            context.Color(ColorRGB(0xFFFFFF));
            PrimitiveGraphic::FillCircle(context, m_ClosestSegmentPoint, s_PointRadius - 2.0f);
        }
    }
}

void TrackEditor::HandleEvent(const Event& event)
{
    if (m_Track == nullptr)
    {
        return;
    }

    switch (event.Type)
    {
    case EventType::MouseButtonPress:
        if (event.Mouse.Button == MOUSE_BUTTON_LEFT)
        {
            int index = LocatePointAt(event.Mouse.Position);

            if (index != -1)
            {
                SelectPoint(index);
            }
            else if (IsPointInCircle(event.Mouse.Position, m_ClosestSegmentPoint, s_SplitRadius))
            {
                SplitSegment(m_ClosestSegmentIndex, m_ClosestSegmentT);
            }
        }
        else if (event.Mouse.Button == MOUSE_BUTTON_RIGHT)
        {
            int index = LocatePointAt(event.Mouse.Position);

            if (index != -1)
            {
                DeletePoint(index);
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
        if (m_SelectedIndex != -1)
        {
            MovePoint(m_SelectedIndex, event.Mouse.Position);
        }
        break;
    case EventType::KeyPress:
        switch (event.Key.Key)
        {
        case 'm':
        case 'M':
            IncreaseWidth();
            break;
        case 'n':
        case 'N':
            DecreaseWidth();
            break;
        case 'o':
        case 'O':
            TogglePreview();
            break;
        }
    default:
        break;
    }
}

void TrackEditor::IncreaseWidth()
{
    m_Width += 5.0f;
}

void TrackEditor::DecreaseWidth()
{
    m_Width = std::max(m_Width - 5.0f, 75.0f);
}

void TrackEditor::TogglePreview()
{
    m_Preview = !m_Preview;
}

int TrackEditor::LocatePointAt(Vector2 position)
{
    for (size_t i = 0; i < m_Points.size(); i++)
    {
        if (IsPointInCircle(position, m_Points[i], s_PointRadius))
        {
            return static_cast<int>(i);
        }
    }

    return -1;
}

void TrackEditor::SelectPoint(int index)
{
    m_SelectedIndex = index;
}

void TrackEditor::DeselectPoint()
{
    m_SelectedIndex = -1;
}

void TrackEditor::MovePoint(size_t index, Vector2 position)
{
    Vector2 previousPosition = m_Points[index];

    m_Points[index] = position;

    if (IsP0OrP1(m_SelectedIndex))
    {
        size_t currentSegmentIndex = GetSegmentIndex(m_SelectedIndex);
        size_t previousSegmentIndex = GetSegmentCount() + currentSegmentIndex - 1;

        Vector2 reference = previousPosition;

        Vector2 tangentC0 = GetC0(currentSegmentIndex) - reference;
        Vector2 tangentC1 = GetC1(previousSegmentIndex) - reference;

        SetC0(currentSegmentIndex, position + tangentC0);
        SetC1(previousSegmentIndex, position + tangentC1);

        return;
    }

    if (IsC0(m_SelectedIndex))
    {
        size_t currentSegmentIndex = GetSegmentIndex(m_SelectedIndex);
        size_t previousSegmentIndex = GetSegmentCount() + currentSegmentIndex - 1;

        Vector2 reference = GetP0(currentSegmentIndex);
        Vector2 tangent = GetC0(currentSegmentIndex) - reference;
        tangent = tangent.Normalized();

        float magnitude = (GetC1(previousSegmentIndex) - reference).Magnitude();

        SetC1(previousSegmentIndex, reference - tangent * magnitude);

        return;
    }

    if (IsC1(m_SelectedIndex))
    {
        size_t currentSegmentIndex = GetSegmentIndex(m_SelectedIndex);
        size_t nextSegmentIndex = currentSegmentIndex + 1;

        Vector2 reference = GetP1(currentSegmentIndex);
        Vector2 tangent = GetC1(currentSegmentIndex) - reference;
        tangent = tangent.Normalized();

        float magnitude = (GetC0(nextSegmentIndex) - reference).Magnitude();

        SetC0(nextSegmentIndex, reference - tangent * magnitude);

        return;
    }
}

void TrackEditor::DeletePoint(size_t index)
{
    if (!IsP0OrP1(index))
    {
        return;
    }

    if (m_Points.size() <= 6)
    {
        return;
    }

    if (index > 0)
    {
        m_Points.erase(m_Points.begin() + (index - 1));
        m_Points.erase(m_Points.begin() + (index - 1));
        m_Points.erase(m_Points.begin() + (index - 1));
    }
    else
    {
        m_Points.emplace_back(m_Points[0]);
        m_Points.emplace_back(m_Points[1]);
        m_Points.emplace_back(m_Points[2]);

        m_Points.erase(m_Points.begin(), m_Points.begin() + 3);

        DeletePoint(m_Points.size() - 3);
    }
}

void TrackEditor::SplitSegment(size_t segmentIndex, float t)
{
    Vector2 p0 = GetP0(segmentIndex);
    Vector2 c0 = GetC0(segmentIndex);
    Vector2 c1 = GetC1(segmentIndex);
    Vector2 p1 = GetP1(segmentIndex);

    Vector2 p01 = Lerp(p0, c0, t);
    Vector2 p12 = Lerp(c0, c1, t);
    Vector2 p23 = Lerp(c1, p1, t);

    Vector2 p012 = Lerp(p01, p12, t);
    Vector2 p123 = Lerp(p12, p23, t);

    Vector2 p0123 = Lerp(p012, p123, t);

    SetC0(segmentIndex, p01);
    SetC1(segmentIndex, p012);

    m_Points.insert(m_Points.begin() + segmentIndex * 3 + 3, { p0123, p123, p23 });
}

int TrackEditor::FindClosestSegmentToPoint(Vector2 point, float& closestT)
{
    float bestT = 0.0f;
    float bestDistance = std::numeric_limits<float>::max();
    int bestSegmentIndex = -1;

    for (size_t segmentIndex = 0; segmentIndex < GetSegmentCount(); ++segmentIndex)
    {
        float t = FindClosestParameterToPoint(point, segmentIndex);

        Vector2 sample = EvaluateSegment(segmentIndex, t);
        float distance = (sample - point).MagnitudeSquared();
        
        if (distance < bestDistance)
        {
            bestT = t;
            bestDistance = distance;
            bestSegmentIndex = static_cast<int>(segmentIndex);
        }
    }

    closestT = bestT;

    return bestSegmentIndex;
}

float TrackEditor::FindClosestParameterToPoint(Vector2 point, size_t segmentIndex)
{
    const int coarseSamples = 100;
    const int fineSamples = 100;

    const float coarseIncrement = 1.0f / coarseSamples;
    const float fineIncrement = 2 * coarseIncrement / fineSamples;
    
    float bestT = 0.0f;
    float bestDistance = std::numeric_limits<float>::max();
    
    for (int i = 0; i <= coarseSamples; i++)
    {
        float t = static_cast<float>(i) / coarseSamples;

        Vector2 sample = EvaluateSegment(segmentIndex, t);
        float distance = (sample - point).MagnitudeSquared();
        
        if (distance < bestDistance)
        {
            bestDistance = distance;
            bestT = t;
        }
    }
    
    float minimumT = std::max(0.0f, bestT - coarseIncrement);

    for (int i = 0; i <= fineSamples; i++) 
    {
        float t = minimumT + static_cast<float>(i) * fineIncrement;

        Vector2 sample = EvaluateSegment(segmentIndex, t);
        float distance = (sample - point).MagnitudeSquared();
        
        if (distance < bestDistance)
        {
            bestDistance = distance;
            bestT = t;
        }
    }
    
    return bestT;
}

Vector2 TrackEditor::EvaluateSegment(size_t segmentIndex, float t)
{
    Vector2 p0 = GetP0(segmentIndex);
    Vector2 c0 = GetC0(segmentIndex);
    Vector2 c1 = GetC1(segmentIndex);
    Vector2 p1 = GetP1(segmentIndex);

    return CubicBezier(p0, c0, c1, p1, t);
}

size_t TrackEditor::GetSegmentIndex(size_t pointIndex)
{
    return pointIndex / 3;
}

size_t TrackEditor::GetSegmentCount()
{
    return m_Points.size() / 3;
}

void TrackEditor::SetP0(size_t segmentIndex, Vector2 point)
{
    m_Points[IndexP0(segmentIndex)] = point;
}

void TrackEditor::SetP1(size_t segmentIndex, Vector2 point)
{
    m_Points[IndexP1(segmentIndex)] = point;
}

void TrackEditor::SetC0(size_t segmentIndex, Vector2 point)
{
    m_Points[IndexC0(segmentIndex)] = point;
}

void TrackEditor::SetC1(size_t segmentIndex, Vector2 point)
{
    m_Points[IndexC1(segmentIndex)] = point;
}

Vector2& TrackEditor::GetP0(size_t segmentIndex)
{
    return m_Points[IndexP0(segmentIndex)];
}

Vector2& TrackEditor::GetP1(size_t segmentIndex)
{
    return m_Points[IndexP1(segmentIndex)];
}

Vector2& TrackEditor::GetC0(size_t segmentIndex)
{
    return m_Points[IndexC0(segmentIndex)];
}

Vector2& TrackEditor::GetC1(size_t segmentIndex)
{
    return m_Points[IndexC1(segmentIndex)];
}

size_t TrackEditor::IndexP0(size_t segmentIndex)
{
    return (m_Points.size() + segmentIndex * 3) % m_Points.size();
}

size_t TrackEditor::IndexP1(size_t segmentIndex)
{
    return (m_Points.size() + segmentIndex * 3 + 3) % m_Points.size();
}

size_t TrackEditor::IndexC0(size_t segmentIndex)
{
    return (m_Points.size() + segmentIndex * 3 + 1) % m_Points.size();
}

size_t TrackEditor::IndexC1(size_t segmentIndex)
{
    return (m_Points.size() + segmentIndex * 3 + 2) % m_Points.size();
}

bool TrackEditor::IsP0OrP1(size_t pointIndex)
{
    return pointIndex % 3 == 0;
}

bool TrackEditor::IsC0(size_t pointIndex)
{
    return pointIndex % 3 == 1;
}

bool TrackEditor::IsC1(size_t pointIndex)
{
    return pointIndex % 3 == 2;
}