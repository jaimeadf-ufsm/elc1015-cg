#include "Message.h"
#include "Scene.h"
#include "TextRuler.h"

Message::Message(std::reference_wrapper<Scene> scene) : 
    GameObject(scene),
    m_Content(""),
    m_Duration(1.0f),
    m_Elapsed(0.0f)
{
}

void Message::Initialize()
{
}

void Message::Update(float deltaTime)
{
    m_Elapsed += deltaTime;

    if (m_Elapsed >= m_Duration)
    {
        Destroy();
    }
}

void Message::Draw(DrawingContext& context)
{
    Scene& scene = GetScene();
    Viewport& viewport = scene.GetViewport();

    ColorRGB backgroundColor = ColorRGB(0xFFFFFF);
    ColorRGB foregroundColor = ColorRGB(0x000000); 

    int textWidth = MeasureTextWidth(m_Content);
    int textHeight = MeasureTextHeight(m_Content);

    int textX = (viewport.GetWidth() - textWidth) / 2;
    int textY = (viewport.GetHeight() - textHeight) / 2;

    context.Color(backgroundColor);
    context.FillRectangle(Vector2(textX - 12, textY - 12), Vector2(textWidth + 24, textHeight + 24));

    context.Color(foregroundColor);
    context.Text(Vector2(textX, textY), m_Content);
}

void Message::SetContent(const std::string& content)
{
    m_Content = content;
}

const std::string& Message::GetContent() const
{
    return m_Content;
}

void Message::SetDuration(float duration)
{
    m_Duration = duration;
}

float Message::GetDuration() const
{
    return m_Duration;
}