#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

#define MINIAUDIO_IMPLEMENTATION
#include "AudioSystem.h"
#include "miniaudio.h"

AudioSystem::AudioSystem()
{
    m_Engine = std::make_shared<ma_engine>();
    ma_result result = ma_engine_init(NULL, m_Engine.get());

    if (result != MA_SUCCESS)
    {
        std::cerr << "Unable to initialize audio engine." << std::endl;
    }
    
    SetVolume(0.3f);
}

AudioSystem::~AudioSystem()
{
    for (auto& entry : m_Sounds)
    {
        ma_sound_uninit(entry.second.get());
    }

    ma_engine_uninit(m_Engine.get());
}

bool AudioSystem::LoadAudio(const std::string &name, const std::string &filePath)
{
    if (m_Sounds.find(name) != m_Sounds.end())
    {
        return false;
    }

    auto sound = std::make_shared<ma_sound>();
    ma_result result = ma_sound_init_from_file(m_Engine.get(), filePath.c_str(), 0, NULL, NULL, sound.get());

    if (result != MA_SUCCESS)
    {
        return false;
    }

    m_Sounds[name] = sound;
    return true;
}

bool AudioSystem::PlayAudio(const std::string &name)
{
    auto it = m_Sounds.find(name);

    if (it == m_Sounds.end())
    {
        return false;
    }

    ma_sound_seek_to_pcm_frame(it->second.get(), 0);
    ma_result result = ma_sound_start(it->second.get());

    if (result != MA_SUCCESS)
    {
        return false;
    }

    return true;
}

void AudioSystem::SetVolume(float volume)
{
    if (m_Engine)
    {
        ma_engine_set_volume(m_Engine.get(), volume);
    }
}
