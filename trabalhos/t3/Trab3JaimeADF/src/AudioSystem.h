#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "AudioSystem.h"

struct ma_engine;
struct ma_sound;

class AudioSystem
{
public:
    AudioSystem();
    ~AudioSystem();

    bool LoadAudio(const std::string &name, const std::string &filePath);
    bool PlayAudio(const std::string &name);
    void SetVolume(float volume);

private:
    std::shared_ptr<ma_engine> m_Engine;
    std::unordered_map<std::string, std::shared_ptr<ma_sound>> m_Sounds;
};
