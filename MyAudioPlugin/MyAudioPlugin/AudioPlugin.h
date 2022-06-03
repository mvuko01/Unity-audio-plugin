#pragma once

#include <fmod.hpp>
#include <map>
#include <string>
#include <vector>
#include "Utility.h"


#ifdef DLL_EXPORTS
#define DllExport __declspec(dllexport) 
#else
#define DllExport __declspec(dllimport) 
#endif

using namespace std;

enum RESULT {
	OK,
	FMOD_ERROR,
	SOUND_ALREADY_LOADED,
	SOUNDLOAD_FAILED,
	SOUND_NOT_FOUND,
	PLAYSOUND_ERROR,
	CHANNEL_NOT_FOUND

};

struct AudioSource {
	Vector3 position;
	float min_Sound_Distance = 5.0f;
	float max_Sound_Distance = 50.0f;
}; // represents a single audio source

struct AudioListener {
	Vector3 position;
	Vector3 forward;
	Vector3 up;
}; // represents an audio listener

class Implementation {
public:
	Implementation();
	~Implementation();
	void Update();

	FMOD::System* system;

	int mnNextChannelId;

	typedef map<string, FMOD::Sound*> SoundMap; //SoundMap stores a sound pointer along with the string path after the sound is loaded
	typedef map<int, FMOD::Channel*> ChannelMap; //ChannelMap stores all of the active channels with their respective ids
	
	
	SoundMap mSounds;
	ChannelMap mChannels;

}; //Class Implementation servers as the backbone of the sistem, where the FMOD system is created and started
 


struct SpatializerData {
	typedef map<int, AudioSource> AudioSourceMap;  // AudioSourceMap stores al audio sources along with the channel ID on which the sound is played


	AudioListener listener;
	AudioSourceMap mSources;

	int numberOfSources = 0;
}; 


extern "C" {

	DllExport void  InitAudioEngine(); 
	DllExport void  UpdateAudioEngine();
	DllExport void  ShutdownAudioEngine();
	

	DllExport int LoadSound(char* strSoundPath, bool bLooping);
	DllExport int UnLoadSound(char* strSoundPath);
	DllExport int PlaySounds(char* strSoundPath);

	//void StopChannel(int nChannelId);
	//void StopAllChannels();
	//bool IsPlaying(int nChannelId) const;
	int ReturnNumOfSounds(); 
	int ReturnNumOfChannels(); 
	
	
	RESULT SetChannelVolume(int nChannelId, float fVolumedB);
	RESULT SetChannelPan(int nChannelId, float panValue);
	float AngleValue(AudioSource localSource);
	

	DllExport int SetListener(Vector3 pos, Vector3 forward, Vector3 up);
	DllExport int SetSources(AudioSource* sourceArray, int size);


	int ChangeVolumeByDistance(int nChannelId, AudioSource localSource);
	int ChangePanByOrientation(int nChannelId, AudioSource localSource);
	DllExport int  SpatializeSourcesAndAudio();

	
	static RESULT ErrorCheck(FMOD_RESULT result);
}


