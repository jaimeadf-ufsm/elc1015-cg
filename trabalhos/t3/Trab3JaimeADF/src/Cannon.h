#pragma once

#include "GameObject.h"
#include "MuzzleFlash.h"

class Cannon : public GameObject
{
public:
    Cannon(std::reference_wrapper<Scene> scene);

    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw(DrawingContext& content) override;

    void Fire();
    void AimAt(const Vector2& position);
    void AimAt(float angle);

    void SetWidth(float width);
    float GetWidth() const;

    void SetFireRate(float rate);
    float GetFireRate() const;

    void SetIgnoredTags(const std::unordered_set<std::string>& tags);
    const std::unordered_set<std::string>& GetIgnoredTags(const std::unordered_set<std::string>& tags) const;

private:
    float m_Width;
    float m_FireRate;

    float m_TimeSinceLastFire;

    std::unordered_set<std::string> m_IgnoredTags;

    std::shared_ptr<MuzzleFlash> m_MuzzleFlash;

    Vector2 GetMuzzlePosition(float offset) const;
};