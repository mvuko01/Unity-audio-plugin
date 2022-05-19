#include <iostream>

#include <fmod_errors.h>
#include <fmod_common.h>
#include <thread>
#include "AudioPlugin.h"

using namespace std;
using namespace FMOD;


Implementation::Implementation() {
	system = NULL;
	ErrorCheck(System_Create(&system));
	ErrorCheck(system->init(2, FMOD_INIT_NORMAL, 0));
	mnNextChannelId = 0;
	
}

Implementation::~Implementation() {
	ErrorCheck(system->release());
	
}

void Implementation::Update() {

	//prva for petlja prolazi kroz mapu mChannels i provjerava dali su prestali sa zvukom 
	//ako su prestali stavlja ih u vektor pStoppedChannels
	vector<ChannelMap::iterator> pStoppedChannels;
	for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
	{
		bool bIsPlaying = false;
		it->second->isPlaying(&bIsPlaying);
		if (!bIsPlaying)
		{
			pStoppedChannels.push_back(it);
		}
	}

	//druga petlja brise one kanale koji su zaustavljeni iz mape
	for (auto& it : pStoppedChannels)
	{
		
		mChannels.erase(it);
	}
	
	//cout << "My sound size: " << mSounds.size() << endl;

	ErrorCheck(system->update());
}


Implementation* sgpImplementation = nullptr;
AudioListener* listener = nullptr;

 void  InitAudioEngine() {
	sgpImplementation = new Implementation;
}

void  UpdateAudioEngine() {
	sgpImplementation->Update();
}

/*void AudioEngine::LoadSound(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream)
{
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt != sgpImplementation->mSounds.end())
		return;
	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= b3d ? FMOD_3D : FMOD_2D;
	eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
	FMOD::Sound* pSound = nullptr;
	AudioEngine::ErrorCheck(sgpImplementation->system->createSound(strSoundName.c_str(), eMode, nullptr, &pSound));
	if (pSound) {
		sgpImplementation->mSounds[strSoundName] = pSound;
	}
}*/

int LoadSound(char* pstrSoundPath, bool bLooping)
{
	string strSoundPath(pstrSoundPath);

	//ukoliko se je vec zvuk ucitan ne radi nista ova funkcija
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundPath);
	if (tFoundIt != sgpImplementation->mSounds.end())
		return -1;


	//definiramo flagove zvuka
	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= FMOD_3D;

	FMOD::Sound* sound = NULL;
	ErrorCheck(sgpImplementation->system->createSound(pstrSoundPath, eMode, 0, &sound)); 

	if (sound) {
		
		sgpImplementation->mSounds[strSoundPath] = sound;
		return 1;
	}
	//auto it = sgpImplementation->mSounds.find(strSoundPath);

	//cout << it->first;
}

 int ReturnNumOfSounds() //funkcija za testiranjes
{
	
	int n =	sgpImplementation->mSounds.size();
	return n;
}

 int ReturnNumOfChannels() //funkcija za testiranjes
 {

	 int n = sgpImplementation->mChannels.size();
	 return n;
 }


void UnLoadSound(char* pstrSoundPath)
{
	string strSoundPath(pstrSoundPath);

	auto tFoundIt = sgpImplementation->mSounds.find(strSoundPath);
	if (tFoundIt == sgpImplementation->mSounds.end())
		return;
	ErrorCheck(tFoundIt->second->release());
	sgpImplementation->mSounds.erase(tFoundIt);
}



/*int AudioEngine::PlaySounds(const string& strSoundName, const Vector3& vPosition, float fVolumedB)
{
	int nChannelId = sgpImplementation->mnNextChannelId++;
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt == sgpImplementation->mSounds.end())
	{
		LoadSound(strSoundName);
		tFoundIt = sgpImplementation->mSounds.find(strSoundName);
		if (tFoundIt == sgpImplementation->mSounds.end())
		{
			return nChannelId;
		}
	}

	FMOD::Channel* pChannel = nullptr;
	AudioEngine::ErrorCheck(sgpImplementation->system->playSound(tFoundIt->second, nullptr, true, &pChannel));
	if (pChannel)
	{
		FMOD_MODE currMode;
		tFoundIt->second->getMode(&currMode);
		if (currMode & FMOD_3D) {
			FMOD_VECTOR position = VectorToFmod(vPosition);
			AudioEngine::ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
		}
		AudioEngine::ErrorCheck(pChannel->setVolume(dbToVolume(fVolumedB)));
		AudioEngine::ErrorCheck(pChannel->setPaused(false));
		sgpImplementation->mChannels[nChannelId] = pChannel;
	}
	return nChannelId;
}*/

int PlaySounds(char* pstrSoundPath, float fVolumedB, const Vector3& vPosition)
{
	string strSoundPath(pstrSoundPath);

	int nChannelId = sgpImplementation->mnNextChannelId++;
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundPath);
	if (tFoundIt == sgpImplementation->mSounds.end())
	{

		LoadSound(pstrSoundPath, false);
		tFoundIt = sgpImplementation->mSounds.find(strSoundPath);

		if (tFoundIt == sgpImplementation->mSounds.end())
		{
			return -1;
		}
		
	}
	//cout << "Found path in map: "<< tFoundIt->first;
	FMOD::Channel* pChannel = nullptr;
	ErrorCheck(sgpImplementation->system->playSound(tFoundIt->second, nullptr, true, &pChannel));
	if (pChannel)
	{
		
		//cout << "Channel is playing";
		/*FMOD_VECTOR position = VectorToFmod(vPosition);
		AudioEngine::ErrorCheck(pChannel->set3DAttributes(&position, nullptr));*/
		
		//ErrorCheck(pChannel->setVolume(dbToVolume(fVolumedB)));
		ErrorCheck(pChannel->setPaused(false));
		sgpImplementation->mChannels[nChannelId] = pChannel;
		
		return nChannelId;
	}
	return -2;
}


void SetChannel3dPosition(int nChannelId, const Vector3& vPosition)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	FMOD_VECTOR position = VectorToFmod(vPosition);
	ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
}

void SetChannelVolume(int nChannelId, float fVolumedB)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	ErrorCheck(tFoundIt->second->setVolume(dbToVolume(fVolumedB)));
}





FMOD_VECTOR VectorToFmod(const Vector3& vPosition) {
	FMOD_VECTOR fVec;
	fVec.x = vPosition.x;
	fVec.y = vPosition.y;
	fVec.z = vPosition.z;
	return fVec;
}

int ErrorCheck(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		cout << "FMOD ERROR " << result << endl;
		return 1;
	}
	// cout << "FMOD all good" << endl;
	return 0;
}

float  dbToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

float  VolumeTodB(float volume)
{
	return 20.0f * log10f(volume);
}

void ShutdownAudioEngine() {
	delete sgpImplementation;
}


void SetListener(Vector3 pos) {
	listener = new AudioListener;
	listener->position = pos;
}

float GetListenerX()
{
	return listener->position.x;
}

float GetListenerY() 
{
	return listener->position.y;
}

float GetListenerZ()
{
	return listener->position.z;
}

/*void ErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
}*/


/*extern "C" {


	 void  DllExport Spatialize(AudioPositionVectors source, AudioPositionVectors listener, char* url, AudioPositionVectors source2, char* url2)
	{

		FMOD_RESULT result;

		System* system = NULL;
		result = System_Create(&system);
		ErrorCheck(result);


		result = system->init(2, FMOD_INIT_NORMAL, 0);
		ErrorCheck(result);


		FMOD::Sound* sound = NULL;
		FMOD::Sound* sound2 = NULL;
		result = system->createSound(url, FMOD_3D_LINEARROLLOFF, 0, &sound);
		ErrorCheck(result);
		
		if (url2 != NULL)
		{
			result = system->createSound(url2, FMOD_3D_LINEARROLLOFF, 0, &sound2);
			ErrorCheck(result);
		}

		FMOD::Channel* channel = NULL;
		FMOD::Channel* channel2 = NULL;
		result = system->playSound(sound, 0, false, &channel);
		ErrorCheck(result);

		if (url2 != NULL)
		{
			result = system->playSound(sound2, 0, false, &channel2);
			ErrorCheck(result);
		}

		channel->set3DAttributes(&source.position, &source.velocity);
		channel->set3DMinMaxDistance(1.0f, 100.0f);

		if (url2 != NULL)
		{
			channel2->set3DAttributes(&source2.position, &source2.velocity);
			channel2->set3DMinMaxDistance(1.0f, 100.0f);
		}

		system->set3DListenerAttributes(0, &listener.position, &listener.velocity, &listener.forward, &listener.up);
		
		bool isPlaying = false;
		do {
			//std::this_thread::sleep_for(std::chrono::milliseconds(10));



			channel->isPlaying(&isPlaying);
			system->update();
			
		} while (isPlaying);
	}







	DllExport void  Initialize(char* url)
	{
		FMOD_RESULT result;

		System* system = NULL;
		result = System_Create(&system);

		ErrorCheck(result);


		result = system->init(100, FMOD_INIT_NORMAL, 0); //prvi broj je koliko kanala odjednom mozes instacirati



		if (result != FMOD_OK)
		{
			printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			exit(-1);
		}

		FMOD::Sound* sound = NULL;
		result = system->createSound(url, FMOD_3D, 0, &sound);
		if (result != FMOD_OK)
		{
			printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			exit(-1);
		}

		FMOD::Channel* channel = NULL;
		result = system->playSound(sound, 0, false, &channel);
		if (result != FMOD_OK)
		{
			printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			exit(-1);
		}

		bool isPlaying = false;
		do {
			//std::this_thread::sleep_for(std::chrono::milliseconds(10));



			channel->isPlaying(&isPlaying);

			system->update();
		} while (isPlaying);

	}

	
}*/