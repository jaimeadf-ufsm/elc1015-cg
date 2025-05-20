#pragma once

#include "GameObject.h"
#include "VectorGraphic.h"

class Message : public GameObject
{
public:
    std::vector<TesselatedGraphic> Frames;

    Message(std::reference_wrapper<Scene> scene);

    virtual void Initialize() override;

    virtual void Update(float deltaTime) override;
    virtual void Draw(DrawingContext& context) override;

    void SetContent(const std::string& content);
    const std::string& GetContent() const;

    void SetDuration(float duration);
    float GetDuration() const;

private:
    std::string m_Content;

    float m_Duration;
    float m_Elapsed;
};