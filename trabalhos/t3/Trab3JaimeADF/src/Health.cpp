#include <algorithm>

#include "Health.h"

Health::Health() :
    m_Value(0.0f),
    m_MaxValue(0.0f),
    m_Invincible(false)
{

}

void Health::Heal(float amount)
{
    SetValue(m_Value + amount);
}

void Health::TakeDamage(float amount)
{
    if (!m_Invincible)
    {
        SetValue(m_Value - amount);
    }
}

float Health::ComputePercetange() const
{
    return m_Value / m_MaxValue;
}

void Health::SetValue(float value)
{
    m_Value = std::max(std::min(value, m_MaxValue), 0.0f);
}

void Health::SetMaxValue(float maxValue)
{
    m_MaxValue = std::max(maxValue, 0.0f);
}

void Health::SetInvincible(bool invincible)
{
    m_Invincible = invincible;
}

float Health::GetValue() const
{
    return m_Value;
}

float Health::GetMaxValue() const
{
    return m_MaxValue;
}

bool Health::IsInvincible() const
{
    return m_Invincible;
}

bool Health::IsZero() const
{
    return m_Value == 0.0f;
}