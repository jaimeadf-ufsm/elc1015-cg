#include "GameState.h"
#include <fstream> // Added for file operations
#include <vector>  // Ensure vector is included (likely via other headers, but good practice)
#include "Track.h" // Ensure Track is included for std::make_shared<Track>

static const size_t s_DefaultNumberOfTracks = 10;

GameState::GameState() :
    SelectedTrackIndex(1),
    Tracks(),
    Scores()
{
    Tracks.resize(s_DefaultNumberOfTracks);

    for (size_t i = 0; i < Tracks.size(); i++)
    {
        Tracks[i] = std::make_shared<Track>();
    }
}

void GameState::SelectTrack(size_t index)
{
    SelectedTrackIndex = index % Tracks.size();
}

std::shared_ptr<Track> GameState::GetSelectedTrack()
{
    return Tracks[SelectedTrackIndex];
}

void GameState::CreateScore(int score)
{
    std::shared_ptr<ScoreRecord> record = std::make_shared<ScoreRecord>();
    record->Score = score;

    Scores.emplace_back(record);
}

void GameState::Save(const std::string& filename)
{
    std::ofstream ofs(filename);

    if (!ofs.is_open())
    {
        throw std::runtime_error("Failed to open file for saving game state.");
    }

    ofs << Tracks.size() << std::endl;

    for (const auto& track : Tracks)
    {
        ofs << track->GetWidth() << std::endl;
        ofs << track->GetPoints().size() << std::endl;

        for (const auto& point : track->GetPoints())
        {
            ofs << point.X << " " << point.Y << std::endl;
        }
    }

    ofs << Scores.size() << std::endl;

    for (const auto& score : Scores)
    {
        ofs << score->Score << std::endl;
    }
}

void GameState::Load(const std::string& filename)
{
    std::ifstream ifs(filename);

    if (!ifs.is_open())
    {
        return;
    }

    size_t trackCount;
    ifs >> trackCount;

    Tracks.clear();
    Tracks.reserve(trackCount);

    for (size_t i = 0; i < trackCount; i++)
    {
        float width;
        ifs >> width;

        size_t pointCount;
        ifs >> pointCount;

        std::vector<Vector2> points(pointCount);

        for (size_t j = 0; j < pointCount; j++)
        {
            ifs >> points[j].X >> points[j].Y;
        }

        Tracks.emplace_back(std::make_shared<Track>(width, points));
    }

    size_t scoreCount;
    ifs >> scoreCount;

    Scores.clear();
    Scores.reserve(scoreCount);

    for (size_t i = 0; i < scoreCount; i++)
    {
        int score;
        ifs >> score;

        std::shared_ptr<ScoreRecord> record = std::make_shared<ScoreRecord>();
        record->Score = score;

        Scores.emplace_back(record);
    }

    ifs.close();
}
