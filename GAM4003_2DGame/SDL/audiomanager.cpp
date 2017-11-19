#include "audiomanager.h"

AudioManager* AudioManager::instance = NULL;

AudioManager* AudioManager::getInstance()
{
	if (instance == NULL)
		instance = new AudioManager();
	return instance;
}

AudioManager::AudioManager()
{
	// Initialise Mixer
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

	// Get some channels
	Mix_AllocateChannels(35);
	Mix_GroupChannels(0, 34, 1);
}

AudioManager::~AudioManager()
{
	// Free mixer resources
	Mix_HaltGroup(1);

	for (MAP_EffectsIT it = soundEffectMap.begin(); it != soundEffectMap.end(); ++it)
			Mix_FreeChunk(it->second);
	
	Mix_FreeMusic(music);	//v1.2
	Mix_CloseAudio();
}

bool AudioManager::addSound(char* filename, string name)
{
	string fn = filename;
	if ((int)fn.find("wav") > 0 || (int)fn.find("WAV") > 0) {
		MAP_EffectsIT it = soundEffectMap.find(name);
		Mix_Chunk* chunk = NULL;

		// See if 'name' already exists
		if (it != soundEffectMap.end()) return false;

		// Attempt to load wav file
		chunk = Mix_LoadWAV(filename);
		if (chunk == NULL) return false;

		Mix_VolumeChunk(chunk, MIX_MAX_VOLUME);
		soundEffectMap[name] = chunk;
	}
	return true;
}

//************************************************	//v1.1
bool AudioManager::addSound(char* filename, string name, int volDivider){
	string fn = filename;
	if (fn.find("wav") > 0 || fn.find("WAV") > 0) {
		MAP_EffectsIT it = soundEffectMap.find(name);
		Mix_Chunk* chunk = NULL;

		// See if 'name' already exists
		if (it != soundEffectMap.end()) return false;

		// Attempt to load wav file
		chunk = Mix_LoadWAV(filename);
		if (chunk == NULL) return false;

		Mix_VolumeChunk(chunk, MIX_MAX_VOLUME / volDivider);
		soundEffectMap[name] = chunk;
	}
	return true;
}

bool AudioManager::addMusic(string mp3FileName){		//v1.2
	if (mp3FileName.find("mp3") > 0 || mp3FileName.find("MP3") > 0) {
		music = Mix_LoadMUS(mp3FileName.c_str());
		if (music == NULL) return false;
	}
	return true;
}

void AudioManager::playMusic(int volDivider){		//v1.2
	if (music != NULL) {
		Mix_Volume(-1, MIX_MAX_VOLUME / volDivider);
		Mix_PlayMusic(music, -1);
//		cout << "\nPlaying music";
	}
}

int AudioManager::playMusicOnChannel(string name, int channel){
	MAP_EffectsIT it = soundEffectMap.find(name);
	if (it != soundEffectMap.end() && channel > 0)
		return Mix_PlayChannel(channel, it->second, -1);
	return 0;
}

void AudioManager::stopMusicOnChannel(int c){
	Mix_HaltChannel(c);
}

void AudioManager::deleteInstance(){		//v1.2
	if (instance != NULL) {
		delete instance;
		instance = NULL;
	}
}
//************************************************
void AudioManager::playSound(string name)
{
	MAP_EffectsIT it = soundEffectMap.find(name);
	if (it != soundEffectMap.end())
		Mix_PlayChannel(-1, it->second, 0);
}

