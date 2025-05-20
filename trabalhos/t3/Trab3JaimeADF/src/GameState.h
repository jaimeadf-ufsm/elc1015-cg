#pragma once

#include <chrono>
#include <memory>

#include "Track.h"

struct ScoreRecord
{
    int Score;
};

class GameState
{
public:
    size_t SelectedTrackIndex;

    std::vector<std::shared_ptr<Track>> Tracks;
    std::vector<std::shared_ptr<ScoreRecord>> Scores;

    GameState();

    void SelectTrack(size_t index);
    std::shared_ptr<Track> GetSelectedTrack();

    void CreateScore(int score);

    void Save(const std::string& filename);
    void Load(const std::string& filename);
};