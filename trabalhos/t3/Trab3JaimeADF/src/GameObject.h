#pragma once

#include <memory>
#include <unordered_set>

#include "DrawingContext.h"
#include "Event.h"

#include "Transform.h"
#include "Body.h"
#include "Collider.h"
#include "Contact.h"
#include "BoundingBox.h"

class Scene;

class GameObject
{
public:
    std::shared_ptr<Transform> Transform;
    std::shared_ptr<Body> Body;

    std::vector<std::shared_ptr<Collider>> Colliders;

    GameObject(std::reference_wrapper<Scene> scene);

    virtual void Initialize();

    virtual void Update(float deltaTime);
    virtual void Draw(DrawingContext& context);

    virtual void HandleEvent(const Event& event);

    virtual void Collide(const Contact& contact);

    virtual void Destroy();

    void AddTag(const std::string& tag);
    void RemoveTag(const std::string& tag);
    bool HasTag(const std::string& tag) const;

    void SetPriority(int priority);
    int GetPriority() const;

    bool HasBeenDestroyed() const;

    BoundingBox GetBoundingBox() const;

    Scene& GetScene() const;

private:
    std::reference_wrapper<Scene> m_Scene;

    int m_Priority;
    std::unordered_set<std::string> m_Tags;
    
    bool m_Destroyed = false;
};