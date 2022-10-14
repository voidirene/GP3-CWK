#include "Audio.h"

Audio::Audio() : music(nullptr)
{
	int audioRate = 22050;
	Uint16 audioFormat = AUDIO_S16SYS;
	int audioChannels = 2;
	int audioBuffers = 4096;

	Mix_OpenAudio(audioRate, audioFormat, audioChannels, audioBuffers);

	if (Mix_OpenAudio(audioRate, audioFormat, audioChannels, audioBuffers) != 0) //check for errors
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize audio: %s", Mix_GetError());
		exit(-1);
	}
}

Audio::~Audio()
{
	SDL_Quit();
}

void Audio::AddNewSound(const char* filePath)
{
	Mix_Chunk* newChunk = Mix_LoadWAV(filePath);

	if (newChunk != nullptr)
	{
		soundEffects.push_back(newChunk);
		std::cout << (soundEffects.size() - 1) << " sound effect has been added, path: " << filePath << '\n';
	}
	else
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to add sound effect: %s", Mix_GetError());
	}
}

void Audio::PlaySound(const int sound) const
{
	if (sound > soundEffects.size() - 1)
	{
		std::cout << "Sound effect out of range" + '\n';
		return;
	}

	Mix_PlayChannel(-1, soundEffects[sound], 0);
	std::cout << "Playing sound effect: " << sound << '\n';
}

void Audio::AddNewBackgroundMusic(const char* filePath)
{
	music = Mix_LoadMUS(filePath);

	if (music == NULL)
	{
		printf("Failed to load background music. SDL_mix Error: %s" + '\n', Mix_GetError());
	}
}

void Audio::PlayBackgroundMusic()
{
	if (Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(music, -1); //Start playing music
	}
}