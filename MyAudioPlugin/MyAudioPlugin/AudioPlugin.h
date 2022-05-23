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
	Vector3 forward;
	Vector3 up;
};

struct AudioSource {
	Vector3 position;
};


extern "C" {

	DllExport void  InitAudioEngine(); //izbrisan static
	DllExport void  UpdateAudioEngine();
	DllExport  void ShutdownAudioEngine();
	static int ErrorCheck(FMOD_RESULT result);



	DllExport int LoadSound(char* strSoundPath, bool bLooping);
	DllExport void UnLoadSound(char* strSoundPath);
	//void Set3dListenerAndOrientation(const Vector3& vPosition, const Vector3& vLook, const Vector3& vUp);
	DllExport int PlaySounds(char* strSoundPath);

	//void StopChannel(int nChannelId);
	DllExport int ReturnNumOfSounds();
	DllExport int ReturnNumOfChannels();

	//void StopAllChannels();
	DllExport void SetChannel3dPosition(int nChannelId, const Vector3& vPosition);
	void SetChannelVolume(int nChannelId, float fVolumedB);
	DllExport void SetChannelPan(int nChannelId, float panValue);
	//bool IsPlaying(int nChannelId) const;
	
	
	FMOD_VECTOR VectorToFmod(const Vector3& vPosition);

	
	/*Moje funkcije*/
	
	DllExport void SetListener(Vector3 pos, Vector3 forward, Vector3 up);
	DllExport void SetSource(Vector3 pos);
	DllExport void SetMinMaxDistance(float min, float max);
	DllExport void ChangeVolumeByDistance(int nChannelId);
	DllExport float ChangePanByOrientation(int nChannelId);

	DllExport float GetListenerY();
	DllExport float GetListenerX();
	DllExport float GetListenerZ();


	/*Utility --- vecina ovih ne treba dllexport*/

	DllExport float VectorDotProduct(Vector3 vec1, Vector3 vec2);

	DllExport Vector3 VectorCrossProduct(Vector3 vec1, Vector3 vec2);
	DllExport Vector3 VectorSubtract(Vector3 vec1, Vector3 vec2);
	DllExport float AngleValue();
	DllExport Vector3 VectorNormalize(Vector3 vec1);
	float VectorMagnitude(Vector3 vec1);
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