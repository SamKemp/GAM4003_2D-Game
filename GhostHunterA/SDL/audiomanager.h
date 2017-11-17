// Copyright Peter Lager / Steve Manning and the University of Bolton
// Do what you like with the program, but please include your
// sourcecode with any distribution and retain my copyright notices
//

// v1.2 addMusic, playMusic functions added to enable an MP3 music file to be played at the SAME time as sounds effects.
//		Also, deleteInstance added.
// v1.1 functions added for SDL2. SM. MP3 should now play too.  Extra dll/lib files added.
// Mix_Music was Mix_Chunk. sdl_mixer.lib now sdl2_mixer.lib in Linker, Input
// Mix_FreeChunk now Mix_FreeMusic
// Mix_LoadWAV now Mix_LoadMUS
// Mix_VolumeChunk(chunk, MIX_MAX_VOLUME) now Mix_Volume(-1, MIX_MAX_VOLUME)
// Mix_PlayChannel now Mix_PlayMusic

#pragma once;

#include<iostream>
#include<map>
#include<string>
using namespace std;

#include "SDL_mixer.h"

typedef map<string, Mix_Chunk*> MAP_Effects;	//****
typedef map<string, Mix_Chunk*>::iterator MAP_EffectsIT;

class AudioManager
{
private:
	// Static variable of type AudioManager pointer
	static AudioManager* instance;

	// All constructors are declared private to prevent
	// it being used to create AudioManager objects
	// from outside the class
	AudioManager();

public:
	// Static (global) method to return the attribute
	// 'instance'. If instance is NULL it creates an
	// object of type AudioManager and stores it address
	// in the static attribute instance
	static AudioManager* getInstance();

	~AudioManager();

public:
	bool addSound(char* WAVfilename, string name);
	bool addSound(char* WAVfilename, string name, int volDivider);	//v1.1
	void playSound(string name);

	void playMusic(int volDivider = 1);	//v1.2
	int playMusicOnChannel(string name, int channel);
	void stopMusicOnChannel(int c);

	void deleteInstance();
	bool addMusic(string mp3FileName);	//v1.2
private:
	// Map container to maintain collection of sounds
	MAP_Effects soundEffectMap;	//****
	Mix_Music *music;	//assumed to be an MP3 file
};
