#pragma once

#include <fmod.hpp>
#include <map>
#include <vector>
#include <string>


#ifdef DLL_EXPORTS
#define DllExport __declspec(dllexport) 
#else
#define DllExport __declspec(dllimport) 
#endif

using namespace std;


struct Vector3 {
	float x;
	float y;
	float z;
};



struct cmp_str //sluzi kako bi u SoundMap mogli koristit char* kao kljuc
{
	bool operator()(char const* a, char const* b) const
	{
		return std::strcmp(a, b) < 0;
	}
};

struct Implementation {
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
	Vector3 orientation;
};


extern "C" {

	DllExport void  InitAudioEngine(); //izbrisan static
	DllExport void  UpdateAudioEngine();
	DllExport  void ShutdownAudioEngine();
	static int ErrorCheck(FMOD_RESULT result);



	DllExport int LoadSound(char* strSoundPath, bool bLooping);
	DllExport void UnLoadSound(char* strSoundPath);
	//void Set3dListenerAndOrientation(const Vector3& vPosition, const Vector3& vLook, const Vector3& vUp);
	DllExport int PlaySounds(char* strSoundPath, float fVolumedB = 0.0f, const Vector3& vPos = Vector3{ 0, 0, 0 });

	//void StopChannel(int nChannelId);
	DllExport int ReturnNumOfSounds();
	DllExport int ReturnNumOfChannels();

	//void StopAllChannels();
	DllExport void SetChannel3dPosition(int nChannelId, const Vector3& vPosition);
	DllExport void SetChannelVolume(int nChannelId, float fVolumedB);
	//bool IsPlaying(int nChannelId) const;

	float dbToVolume(float dB);
	float VolumeTodB(float volume);
	FMOD_VECTOR VectorToFmod(const Vector3& vPosition);

	DllExport void SetListener(Vector3 pos);
	DllExport float GetListenerY();
	DllExport float GetListenerX();
	DllExport float GetListenerZ();

}



/*extern "C" {

	typedef struct {
	FMOD_VECTOR position;
	FMOD_VECTOR velocity;
	FMOD_VECTOR direction;
	FMOD_VECTOR forward;
	FMOD_VECTOR up;
} AudioPositionVectors;
	DllExport void Spatialize(AudioPositionVectors source, AudioPositionVectors listener, char* url, AudioPositionVectors source2, char* url2);
	DllExport void  Initialize(char* url);

}*/