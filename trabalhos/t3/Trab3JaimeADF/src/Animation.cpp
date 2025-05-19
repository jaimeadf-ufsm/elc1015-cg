#include "Animation.h"

Animation::Animation(std::reference_wrapper<Game> game) : GameObject(game)
{
}

void Animation::Initialize()
{
    m_Duration = 0.0f;
    m_Elapsed = 0.0f;
}

void Animation::Update(float deltaTime)
{
    m_Elapsed += deltaTime;

    if (m_Elapsed >= m_Duration)
    {
        Destroy();
    }
}

void Animation::Draw(DrawingContext& context)
{
    float frameDuration = m_Duration / Frames.size();
    size_t frameIndex = m_Elapsed / frameDuration;

    if (frameIndex < Frames.size())
    {
        Frames[frameIndex].Draw(context, *Transform);
    }
}

void Animation::SetDuration(float duration)
{
    m_Duration = duration;
}

float Animation::GetDuration() const
{
    return m_Duration;
}