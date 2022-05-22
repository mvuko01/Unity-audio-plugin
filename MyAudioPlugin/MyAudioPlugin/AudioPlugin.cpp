#include <iostream>

#include <fmod_errors.h>
#include <fmod_common.h>
#include <thread>
#include "AudioPlugin.h"

using namespace std;
using namespace FMOD;


Implementation* sgpImplementation = nullptr;
AudioListener listener;
AudioSource source;
float min_Sound_Distance;
float max_Sound_Distance;


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




 void  InitAudioEngine() {
	sgpImplementation = new Implementation;
	min_Sound_Distance = 5.0f;
	max_Sound_Distance = 35.0f;
}

void  UpdateAudioEngine() {
	sgpImplementation->Update();
}

void ShutdownAudioEngine() {
	delete sgpImplementation;
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
	return -2;
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
		
		ErrorCheck(pChannel->setVolume(fVolumedB));
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

void SetChannelVolume(int nChannelId, float volume_value)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;


	ErrorCheck(tFoundIt->second->setVolume(volume_value));
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

void ChangeVolumeByDistance(int nChannelId)
{
	float distance = 0;
	float volume = 0;

	distance = sqrt(pow(listener.position.x - source.position.x, 2) + pow(listener.position.y - source.position.y, 2) + pow(listener.position.z - source.position.z, 2));
	if (distance <= min_Sound_Distance)
		volume = 1.2f;
	else if (distance >= max_Sound_Distance)
		volume = 0.0f;
	else
		volume = 1  - ((distance - min_Sound_Distance) / (max_Sound_Distance - min_Sound_Distance));
	
	SetChannelVolume(nChannelId, volume);
	return;
}



void SetListener(Vector3 pos, Vector3 forward, Vector3 up) {
	
	listener.position= pos;
	listener.forward = forward;
	listener.up = up;
}

void SetSource(Vector3 pos) {
	source.position = pos;
}

void SetMinMaxDistance(float min, float max) {
	min_Sound_Distance = min;
	max_Sound_Distance = max;
}


float VectorDotProduct(Vector3 vec1, Vector3 vec2)
{
	float product;

	product = vec1.x * vec2.x;
	product += vec1.z * vec2.z;
	product += vec1.y * vec2.y;


	return product;
}

Vector3 VectorCrossProduct(Vector3 vec1, Vector3 vec2)
{
	Vector3 product;

	product.x = vec1.y * vec2.z - vec1.z * vec2.y;
	product.y = vec1.x * vec2.z - vec1.z * vec2.x;
	product.z = vec1.x * vec2.y - vec1.y * vec2.x;

	return product;

}

float AngleValue()
{
	Vector3 side = VectorCrossProduct(listener.up, listener.forward);
	side = VectorNormalize(side); 
	Vector3 difference = VectorSubtract(source.position, listener.position);
	float x = VectorDotProduct(difference, side);
	float z = VectorDotProduct(difference, listener.forward);
	float angleRadians = atan2(x, z);
	float angleDegrees = angleRadians * (180.0 / 3.14); 

	/*Vector3 difference = VectorSubtract(source.position, listener.position);
	cout << "Difference: " << difference.x << " " << difference.y << " " << difference.z << endl;
	

	float dotProduct = VectorDotProduct(difference, listener.forward);
	cout << "Dot product = " << dotProduct << endl;
	float magnitudeOfDiff = VectorMagnitude(difference);
	cout << "Diff magnitude = " << magnitudeOfDiff << endl;

	float magnitudeOfForward = VectorMagnitude(listener.forward);
	cout << "Forward magnitude = " << magnitudeOfForward << endl;

	float acosArgument = dotProduct / (magnitudeOfDiff * magnitudeOfForward);
	cout << "Acos argument = " << acosArgument << endl;

	float angleRadians = acos(acosArgument);
	float angleDegrees = angleRadians * (180.0 / 3.14);*/
	
	/*Vector3 difference = VectorSubtract(source.position, listener.position);

	float angleRadians = atan2(difference.z * listener.forward.x - difference.x * listener.forward.z, difference.x * listener.forward.x + difference.z * listener.forward.z);
	float angleDegrees = angleRadians * (180.0 / 3.14);*/

	return angleDegrees;
}

float VectorMagnitude(Vector3 vec1)
{
	return sqrtf(vec1.x * vec1.x + vec1.y * vec1.y + vec1.z * vec1.z); //vec1.y * vec1.y +
	 
}

Vector3 VectorNormalize(Vector3 vec1)
{
	Vector3 normalizedVector = {0,0,0};
	float magnitude = VectorMagnitude(vec1);
	if (magnitude > 0)
	{
		normalizedVector.x = vec1.x / magnitude;
		normalizedVector.y = vec1.y / magnitude;
		normalizedVector.z = vec1.z / magnitude;

		return normalizedVector;
	}
	return normalizedVector;
}

Vector3 VectorSubtract(Vector3 vec1, Vector3 vec2)
{
	Vector3 rezult;
	rezult.x = vec1.x - vec2.x;
	rezult.y = vec1.y - vec2.y;
	rezult.z = vec1.z - vec2.z;

	return rezult;
}


float GetListenerX()
{
	return listener.position.x;
}

float GetListenerY() 
{
	return listener.position.y;
}

float GetListenerZ()
{
	return listener.position.x;
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