#pragma once

class Health
{
public:
    Health();

    void Heal(float amount);
    void TakeDamage(float amount);

    float ComputePercetange() const;

    void SetValue(float value);
    void SetMaxValue(float maxValue);

    void SetInvincible(bool invincible);

    float GetValue() const;
    float GetMaxValue() const;

    bool IsInvincible() const;

    bool IsZero() const;
    
private:
    float m_Value;
    float m_MaxValue;

    bool m_Invincible;
};