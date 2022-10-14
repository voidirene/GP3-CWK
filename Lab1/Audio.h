#pragma once
#include <SDL\SDL.h>
#include <SDL\SDL_mixer.h>
#include <iostream>
#include <vector>

class Audio
{
public:

	Audio();
	~Audio();

	void AddNewSound(const char* filePath);
	void AddNewBackgroundMusic(const char* filePath);
	void PlaySound(const int sound) const;
	void PlayBackgroundMusic();

private:

	Mix_Music* music;
	std::vector<Mix_Chunk*> soundEffects;
};

