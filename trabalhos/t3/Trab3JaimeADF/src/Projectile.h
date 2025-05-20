#pragma once

#include "GameObject.h"
#include "VectorGraphic.h"

class Projectile : public GameObject
{
public:
    Projectile(std::reference_wrapper<Scene> scene);

    void Initialize() override;

    void Update(float deltaTime) override;
    void Draw(DrawingContext &context) override;

    void Collide(const Contact& contact) override;

    void SetDamage(float amount);
    float GetDamage() const;

    void AddIgnoredTag(const std::string& tag);
    void RemoveIgnoredTag(const std::string& tag);
    bool HasIgnoredTag(const std::string& tag) const;

private:
    TesselatedGraphic m_Graphic;
    float m_Damage;

    std::unordered_set<std::string> m_IgnoredTags;
};