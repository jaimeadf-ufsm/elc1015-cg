#include <cstdlib>

#include "GameOver.h"

#include "Scene.h"
#include "Constants.h"

#include "Tesselator.h"
#include "TextRuler.h"
#include "Player.h"
#include "Score.h"

GameOver::GameOver(std::reference_wrapper<Scene> scene) :
    GameObject::GameObject(scene),
    m_Over(false)
{
}

void GameOver::Initialize()
{
    GameObject::Initialize();
    SetPriority(1001);
}

void GameOver::Update(float deltaTime)
{
    GameObject::Update(deltaTime);

    Scene& scene = GetScene();
    GameState& state = scene.GetState();

    if (m_Over)
    {
        return;
    }

    if (!IsPlayerAlive())
    {
        m_Over = true;

        std::shared_ptr<Score> score = scene.GetObject<Score>();

        if (score)
        {
            state.CreateScore(score->GetValue());
        }
    }
}

void GameOver::Draw(DrawingContext& context)
{
    GameObject::Draw(context);

    if (!m_Over)
    {
        return;
    }

    Scene& scene = GetScene();
    Viewport& viewport = scene.GetViewport();

    std::string text = "Game Over | Aperte <ESC> para sair";

    ColorRGB backgroundColor = ColorRGB(0xFFFFFF);
    ColorRGB foregroundColor = ColorRGB(0x000000); 

    int textWidth = MeasureTextWidth(text);
    int textHeight = MeasureTextHeight(text);

    int textX = (viewport.GetWidth() - textWidth) / 2;
    int textY = (viewport.GetHeight() - textHeight) / 2;

    context.Color(backgroundColor);
    context.FillRectangle(Vector2(textX - 12, textY - 12), Vector2(textWidth + 24, textHeight + 24));

    context.Color(foregroundColor);
    context.Text(Vector2(textX, textY), text);
}

bool GameOver::IsPlayerAlive()
{
    Scene& scene = GetScene();

    std::shared_ptr<Player> player = scene.GetObject<Player>();

    if (player)
    {
        return !player->HasBeenDestroyed();
    }

    return false;
}

bool GameOver::IsOver() const
{
    return m_Over;
}