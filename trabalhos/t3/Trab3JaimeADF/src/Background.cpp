#include "Background.h"

#include "Scene.h"
#include "Constants.h"

#include "Tesselator.h"

Background::Background(std::reference_wrapper<Scene> scene) : GameObject::GameObject(scene)
{
}

void Background::Initialize()
{
    GameObject::Initialize();
    SetPriority(-1);
    SetTrack(Track());
}

void Background::Update(float deltaTime)
{
    GameObject::Update(deltaTime);
}

void Background::Draw(DrawingContext& context)
{
    GameObject::Draw(context);
    
    Scene& scene = GetScene();
    Viewport& viewport = scene.GetViewport();

    context.Color(0x39C272);
    context.FillRectangle(Vector2(0.0f, 0.0f), Vector2(viewport.GetWidth(), viewport.GetHeight()));

    m_Graphic.Draw(context, *Transform);
}

void Background::SetTrack(const Track& track)
{
    m_Track = track;
    m_Graphic = m_Track.ComputeGraphic();
}

const Track& Background::GetTrack() const
{
    return m_Track;
}