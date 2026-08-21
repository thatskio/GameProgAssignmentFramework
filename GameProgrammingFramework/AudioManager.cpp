#include "AudioManager.h"

const std::string SFX_FOLDER_PATH = "Assets/Sounds";

AudioManager::AudioManager()
{
    system = nullptr;
}

AudioManager::~AudioManager()
{
    for (auto& pair : soundRegistry) {
        if (pair.second) {
            pair.second->release();
        }
    }
    soundRegistry.clear();

    if (system) {
        system->close();
        system->release();
    }
}

void AudioManager::InitializeAudio()
{
    result = FMOD::System_Create(&system);
    result = system->init(32, FMOD_INIT_NORMAL, extraDriverData);
}

void AudioManager::LoadSounds(const std::string& key, const std::string& fileName, bool isStream)
{
    if (!system) { std::cout << "System failed to load in AudioManager.LoadSounds!"; return; }

    std::string fullPath = SFX_FOLDER_PATH + "/" + fileName;
    FMOD::Sound* newSound = nullptr;

    if (isStream)
    {
        result = system->createStream(fullPath.c_str(), FMOD_DEFAULT, 0, &newSound);
    }
    else
    {
        result = system->createSound(fullPath.c_str(), FMOD_DEFAULT, 0, &newSound);
    }

    if (newSound)
    {
        newSound->setMode(FMOD_LOOP_OFF);
        soundRegistry[key] = newSound;
    }
}

FMOD::Channel* AudioManager::PlayAudio(const std::string& key, SoundParams params)
{
    if (!system) { std::cout << "System failed to load in AudioManager.PlaySound!"; return nullptr; }

    if (soundRegistry.find(key) != soundRegistry.end())
    {
        result = system->playSound(soundRegistry[key], 0, true, &channel);
        if (result == FMOD_OK && channel)
        {
            channel->setVolume(params.volume);
            channel->setPitch(params.pitch);
            channel->setPan(params.pan);
            channel->setMode(params.loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
            channel->setPaused(false);
        }
    }
    return channel;
}

void AudioManager::UpdateSound()
{
    if (system) system->update();
}