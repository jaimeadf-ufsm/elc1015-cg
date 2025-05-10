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
    if (m_Points.size() > 0)
    {
        Path path;

        path.Initialize(m_Points[0]);

        for (size_t i = 1; i + 2 <= m_Points.size(); i += 3)
        {
            Vector2 c0 = m_Points[i];
            Vector2 c1 = m_Points[i + 1];
            Vector2 p = m_Points[(i + 2) % m_Points.size()];

            path.CubicBezierTo(c0, c1, p);
        }

        path.Close();


        std::vector<Triangle> triangles;

        Tesselator::Stroke(path, triangles, 5.0f, 10.0f);

        context.Color(ColorRGB::Black);

        for (const auto& triangle : triangles)
        {
            context.BeginPolygon();
            context.Vertex(triangle.A);
            context.Vertex(triangle.B);
            context.Vertex(triangle.C);
            context.FillPolygon();
        }

        const std::vector<Vector2>& points = path.GetPoints();

        for (size_t i = 1; i < points.size(); i++)
        {
            Vector2 d = points[i] - points[i - 1];

            std::cout << d.Magnitude() << ", ";
        }

        std::cout << std::endl;

        if (m_Fill)
        {
            Tesselator::Fill(path, triangles);

            context.Color(ColorRGB::Cyan);

            for (const auto& triangle : triangles)
            {
                context.BeginPolygon();
                context.Vertex(triangle.A);
                context.Vertex(triangle.B);
                context.Vertex(triangle.C);
                context.FillPolygon();
            }

        }
    }

    for (size_t i = 0; i < m_Points.size(); i++)
    {
        int r = i % 3;

        if (r == 0)
        {
            context.Color(ColorRGB::Red);
        }
        else if (r == 1)
        {
            context.Color(ColorRGB::Green);
        }
        else
        {
            context.Color(ColorRGB::Blue);
        }

        Vector2 point = m_Points[i];

        context.FillRectangle(point - Vector2(2.0f, 2.0f), Vector2(4.0f, 4.0f));
    }
}

void EditorScene::HandleEvent(const Event& event)
{
    switch (event.Type)
    {
        case EventType::MouseButtonPress:
            std::cout << "Mouse button press" << event.Mouse.Button << std::endl;

            if (event.Mouse.Button == MOUSE_BUTTON_LEFT)
            {
                for (size_t i = 0; i < m_Points.size(); ++i)
                {
                    if (IsInCircle(event.Mouse.Position, m_Points[i], 10.0f))
                    {
                        m_SelectedPoint = static_cast<int>(i);
                        std::cout << "Selected point: " << m_SelectedPoint << std::endl;
                        break;
                    }
                }

                if (m_SelectedPoint == -1)
                {
                    Vector2 point = event.Mouse.Position;

                    std::cout << "New: " << point << std::endl;

                    if (m_Points.empty())
                    {

                        m_Points.emplace_back(point);
                        m_Points.emplace_back(point + Vector2(0.0f, -50.0f));
                        m_Points.emplace_back(point + Vector2(0.0f, +50.0f));
                    }
                    else
                    {
                        Vector2 previousPoint = m_Points[m_Points.size() - 3];
                        Vector2 nextPoint = m_Points[0];

                        std::cout << "Previous: " << previousPoint << std::endl;
                        std::cout << "Next: " << nextPoint << std::endl;

                        Vector2 previousDirection = (previousPoint - point).Normalize();
                        Vector2 nextDirection = (nextPoint - point).Normalize();

                        

                        Vector2 bisector = (previousDirection + nextDirection).Normalize();
                        Vector2 normal = Vector2(-bisector.Y, bisector.X);
                        normal *= (nextPoint - previousPoint).Magnitude() * 0.25f;

                        Vector2 c1 = m_Points[m_Points.size() - 1];


                        m_Points[m_Points.size() - 1] = point + normal;

                        m_Points.emplace_back(point);
                        m_Points.emplace_back(point - normal);
                        m_Points.emplace_back(c1);
                    }
                }
            }
            if (event.Mouse.Button == MOUSE_BUTTON_RIGHT)
            {
                std::cout << "Right click" << std::endl;

            }
            break;
        case EventType::MouseButtonRelease:
            if (event.Mouse.Button == MOUSE_BUTTON_LEFT)
            {
                m_SelectedPoint = -1;
            }
            break;
        case EventType::MouseMove:
            if (m_SelectedPoint != -1)
            {
                m_Points[m_SelectedPoint] = event.Mouse.Position;

                if (m_SelectedPoint % 3 == 1)
                {
                    Vector2 tangent = m_Points[m_SelectedPoint] - m_Points[m_SelectedPoint - 1];

                    m_Points[(m_SelectedPoint - 2 + m_Points.size()) % m_Points.size()] = m_Points[m_SelectedPoint - 1] - tangent;
                }

            }
            break;
        default:
            break;
    }
}

