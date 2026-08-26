#pragma once
#include "fmod.hpp"
#include <string>
#include <iostream>
#include <unordered_map>

struct SoundParams {
	float volume = 1.0f;
	float pitch = 1.0f;
	float pan = 0.0f;
	bool loop = false;
};

class AudioManager
{
private:
	FMOD::System* system;												//Soundcard
	std::unordered_map<std::string, FMOD::Sound*> soundRegistry;		//Sound objects stored in an array
	FMOD::Channel* channel = 0;			//LIKE sprite brush (An interface to choose which sound channel to play sound files)
	FMOD_RESULT		result;				//Like HRESULT (returns succeed or fail)
	void* extraDriverData = nullptr;

public:
	AudioManager();
	~AudioManager();

	void InitializeAudio();
	FMOD::Channel* PlayAudio(const std::string& key, SoundParams params = SoundParams());
	void LoadSounds(const std::string& key, const std::string& fileName, bool isStream = false);
	void UpdateSound();
};