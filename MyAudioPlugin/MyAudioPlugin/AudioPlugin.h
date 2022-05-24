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



class Implementation {
public:
	Implementation();
	~Implementation();
	void Update();

	FMOD::System* system;

	int mnNextChannelId;

	typedef map<string, FMOD::Sound*> SoundMap;
	typedef map<int, FMOD::Channel*> ChannelMap;
	SoundMap mSounds;
	ChannelMap mChannels;

};

struct AudioListener {
	Vector3 position;
	Vector3 forward;
	Vector3 up;
};

struct AudioSource {
	Vector3 position;
};

struct SpatializerData {
	AudioListener listener;
	AudioSource source;
	float min_Sound_Distance;
	float max_Sound_Distance;
	AudioSource* pSourceArray;
	int numberOfSources;
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
	float AngleValue();
	
	
	FMOD_VECTOR VectorToFmod(const Vector3& vPosition); //mislim da ovu funkciju niti ne korismi nigdi

	
	/*Moje funkcije*/
	
	DllExport void SetListener(Vector3 pos, Vector3 forward, Vector3 up);
	DllExport void SetSource(Vector3 pos);
	DllExport void SetMinMaxDistance(float min, float max);
	DllExport void ChangeVolumeByDistance(int nChannelId);
	DllExport float ChangePanByOrientation(int nChannelId);

	DllExport int SetSources(Vector3* array, int size);
	

	
}


