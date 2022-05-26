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

	DllExport void  InitAudioEngine(); //izbrisan static
	DllExport void  UpdateAudioEngine();
	DllExport  void ShutdownAudioEngine();
	static int ErrorCheck(FMOD_RESULT result);



	DllExport int LoadSound(char* strSoundPath, bool bLooping);
	DllExport void UnLoadSound(char* strSoundPath);
	
	DllExport int PlaySounds(char* strSoundPath);

	//void StopChannel(int nChannelId);
	//void StopAllChannels();
	//bool IsPlaying(int nChannelId) const;

	DllExport int ReturnNumOfSounds(); //ove su sluzile za testiranje, ne treba dllexport
	DllExport int ReturnNumOfChannels(); //

	
	void SetChannelVolume(int nChannelId, float fVolumedB);
	void SetChannelPan(int nChannelId, float panValue);
	float AngleValue(AudioSource localSource);
	
	
	FMOD_VECTOR VectorToFmod(const Vector3& vPosition); //mislim da ovu funkciju niti ne korismi nigdi

	
	/*Moje funkcije*/
	
	DllExport void SetListener(Vector3 pos, Vector3 forward, Vector3 up);
	DllExport int SetSources(AudioSource* sourceArray, int size);

	
	DllExport void ChangeVolumeByDistance(int nChannelId, AudioSource localSource);
	DllExport float ChangePanByOrientation(int nChannelId, AudioSource localSource);

	
	
	DllExport string GetPathFromChannel(int channelId); // mozda ne treba uopce
	DllExport int  SpatializeSourcesAndAudio();

	
	
	
}


