#include <cstdlib>

#include "Score.h"

#include "Scene.h"
#include "Constants.h"

#include "Tesselator.h"
#include "TextRuler.h"

Score::Score(std::reference_wrapper<Scene> scene) :
    GameObject::GameObject(scene),
    m_Score(0)
{
}

void Score::Initialize()
{
    GameObject::Initialize();

    SetPriority(1000);
    Transform->SetPosition(Vector2(16.0f, 16.0f));
}

void Score::Update(float deltaTime)
{
    GameObject::Update(deltaTime);
}

void Score::Draw(DrawingContext& context)
{
    GameObject::Draw(context);

    std::string text = "Score:" + std::to_string(m_Score);

    ColorRGB backgroundColor = ColorRGB(0xFFFFFF);
    ColorRGB foregroundColor = ColorRGB(0x000000); 

    Vector2 position = Transform->GetPosition();

    int textHeight = MeasureTextHeight(text);

    context.Color(backgroundColor);
    context.FillRectangle(Vector2(position.X, position.Y), Vector2(256, textHeight + 24));

    context.Color(foregroundColor);
    context.Text(Vector2(position.X + 12, position.Y + 12 + textHeight / 2), text);
}

void Score::IncrementScore(int amount)
{
    m_Score += amount;
}

int Score::GetValue() const
{
    return m_Score;
}