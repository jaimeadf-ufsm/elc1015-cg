#pragma once

#include "GameObject.h"
#include "Track.h"

class TrackEditor : public GameObject
{
public:
    TrackEditor(std::reference_wrapper<Scene> scene);

    void Initialize() override;

    void Update(float deltaTime) override;
    void Draw(DrawingContext& context) override;

    void HandleEvent(const Event& event) override;

private:
    bool m_Preview;
    std::shared_ptr<Track> m_Track;

    std::vector<Vector2> m_Points;

    float m_Width;
    Path m_CurvePath;

    Path m_TemporaryPath;
    Mesh m_TemporaryMesh;

    int m_SelectedIndex;

    int m_ClosestSegmentIndex;
    float m_ClosestSegmentT;
    Vector2 m_ClosestSegmentPoint;

    void IncreaseWidth();
    void DecreaseWidth();

    void TogglePreview();

    int LocatePointAt(Vector2 position);

    void SelectPoint(int index);
    void DeselectPoint();

    void MovePoint(size_t index, Vector2 position);
    void DeletePoint(size_t index);

    void SplitSegment(size_t segmentIndex, float t);

    int FindClosestSegmentToPoint(Vector2 point, float& closestT);
    float FindClosestParameterToPoint(Vector2 point, size_t segmentIndex);

    Vector2 EvaluateSegment(size_t segmentIndex, float t);

    size_t GetSegmentIndex(size_t pointIndex);
    size_t GetSegmentCount();

    Vector2& GetP0(size_t segmentIndex);
    Vector2& GetP1(size_t segmentIndex);
    Vector2& GetC0(size_t segmentIndex);
    Vector2& GetC1(size_t segmentIndex);

    void SetP0(size_t segmentIndex, Vector2 point);
    void SetP1(size_t segmentIndex, Vector2 point);
    void SetC0(size_t segmentIndex, Vector2 point);
    void SetC1(size_t segmentIndex, Vector2 point);

    size_t IndexP0(size_t segmentIndex);
    size_t IndexP1(size_t segmentIndex);
    size_t IndexC0(size_t segmentIndex);
    size_t IndexC1(size_t segmentIndex);

    bool IsP0OrP1(size_t pointIndex);
    bool IsC0(size_t pointIndex);
    bool IsC1(size_t pointIndex);
};
