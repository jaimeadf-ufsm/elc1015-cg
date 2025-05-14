#include <iostream>

#include "EditorScene.h"
#include "Path.h"
#include "Tesselator.h"


bool IsInCircle(Vector2 point, Vector2 center, float radius)
{
    return (point - center).MagnitudeSquared() <= radius * radius;
}

EditorScene::EditorScene(std::shared_ptr<Mouse> mouse, std::shared_ptr<Keyboard> keyboard)
    : m_Mouse(mouse), m_Keyboard(keyboard)
{
    // m_Points = {
    //     Vector2(456, 402),
    //     Vector2(630, 203),
    //     Vector2(786, 447),
    //     Vector2(688, 394),
    //     Vector2(642, 381),
    //     Vector2(589, 387),
    // };
}

void EditorScene::Update(float deltaTime)
{
    // 0 q0
    // 1 c00
    // 2 c01
    // 3 q1
    // 4 c10
    // 5 c11
    // 6 q2
    // 7 c20
    // 8 c21
    // c10 = q1 - (q1 - c01)
    //  q0


    // for (size_t i = 3; i + 3 < m_Points.size(); i += 3)
    // {
    //     Vector2 tangent = m_Points[i] - m_Points[i - 1];
    //     m_Points[i + 1] = m_Points[i] + tangent;
    // }
}

void EditorScene::Render(DrawingContext& context)
{
    static Path path;
    static Mesh mesh1;
    static Mesh mesh2;

    if (m_Points.size() == 4)
    {

        // path.MoveTo(m_Points[0]);

        // for (size_t i = 1; i + 3 < m_Points.size(); i += 3)
        // {
        //     path.CubicBezierTo(m_Points[i], m_Points[i + 1], m_Points[i + 2]);
        // }

        path.Clear();
        path.Arc(Vector2(250, 250), 100.0f, 0.0f, 2 * 3.1415);
        path.Close();

        Tesselator::Fill(mesh1, path);
        Tesselator::Stroke(mesh2, path, 4.0f);
    }

    context.Color(ColorRGB::Gray);
    context.FillMesh(mesh1);

    context.Color(ColorRGB::Black);
    context.FillMesh(mesh2);

    context.Color(ColorRGB::Black);

    for (auto& point : m_Points)
    {
        context.FillRectangle(point - Vector2(2, 2), Vector2(4, 4));
    }
}

void EditorScene::HandleEvent(const Event& event)
{
    switch (event.Type)
    {
        case EventType::MouseButtonPress:
            if (event.Mouse.Button == MOUSE_BUTTON_LEFT)
            {
                m_Points.emplace_back(event.Mouse.Position);
            }
            break;
        case EventType::MouseButtonRelease:
            break;
        case EventType::MouseMove:
            break;
        default:
            break;
    }
}

