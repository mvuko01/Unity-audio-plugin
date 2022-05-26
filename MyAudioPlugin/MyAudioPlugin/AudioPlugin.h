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
};

struct AudioListener {
	Vector3 position;
	Vector3 forward;
	Vector3 up;
};

class Implementation {
public:
	Implementation();
	~Implementation();
	void Update();

	FMOD::System* system;

	int mnNextChannelId;

	typedef map<string, FMOD::Sound*> SoundMap; //svaki sound map je definiran sa strinom i pokazivacem
	typedef map<int, FMOD::Channel*> ChannelMap; //svaki kanal ima svoj int tj. channel id
	
	
	SoundMap mSounds;
	ChannelMap mChannels;

};


struct SpatializerData {
	AudioListener listener;
	AudioSource source;

	

	typedef map<int, AudioSource> AudioSourceMap;  // channel id tj. zvuk kanala povezujemo preko id kanala i AudioSourcea
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
	int ReturnNumOfSounds(); //ove su sluzile za testiranje, ne treba dllexport
	int ReturnNumOfChannels(); //
	

	
	RESULT SetChannelVolume(int nChannelId, float fVolumedB);
	RESULT SetChannelPan(int nChannelId, float panValue);
	float AngleValue(AudioSource localSource);
	

	
	DllExport int SetListener(Vector3 pos, Vector3 forward, Vector3 up);

	DllExport int ChangeVolumeByDistance(int nChannelId, AudioSource localSource);
	DllExport int ChangePanByOrientation(int nChannelId, AudioSource localSource);
	DllExport int SetSources(AudioSource* sourceArray, int size);
	DllExport int  SpatializeSourcesAndAudio();

	
	static RESULT ErrorCheck(FMOD_RESULT result);
}


