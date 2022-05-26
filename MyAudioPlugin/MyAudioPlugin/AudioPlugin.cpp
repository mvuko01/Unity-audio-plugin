#include <iostream>
#include <fmod_errors.h>
#include <fmod_common.h>
#include <thread>
#include "AudioPlugin.h"

using namespace std;
using namespace FMOD;


Implementation* sgpImplementation = nullptr;
SpatializerData spatializerData;



Implementation::Implementation() {
	system = NULL;
	ErrorCheck(System_Create(&system));
	ErrorCheck(system->init(10, FMOD_INIT_NORMAL, 0));
	
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
	

	for (auto& it : pStoppedChannels)
	{
		
		mChannels.erase(it);
	}
	
	ErrorCheck(system->update());
}


 void  InitAudioEngine() {
	sgpImplementation = new Implementation;
	spatializerData.min_Sound_Distance = 5.0f;
	spatializerData.max_Sound_Distance = 35.0f;
}

void  UpdateAudioEngine() {
	sgpImplementation->Update();
}

void ShutdownAudioEngine() {
	delete sgpImplementation;
	
}



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
	eMode |= FMOD_2D;

	FMOD::Sound* sound = NULL;
	ErrorCheck(sgpImplementation->system->createSound(pstrSoundPath, eMode, 0, &sound)); 

	if (sound) {
		
		sgpImplementation->mSounds[strSoundPath] = sound;
		return 1;
	}
	
	return -2;
	
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





int PlaySounds(char* pstrSoundPath)
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

	
	FMOD::Channel* pChannel = nullptr;
	ErrorCheck(sgpImplementation->system->playSound(tFoundIt->second, nullptr, true, &pChannel));
	if (pChannel)
	{		
		ErrorCheck(pChannel->setPaused(false));
		sgpImplementation->mChannels[nChannelId] = pChannel;

		
		return nChannelId;
	}
	return -2;
}



void SetChannelVolume(int nChannelId, float volume_value)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId); //ovo se moze stavit u funkciju pošto se ponavlja èesto
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;


	ErrorCheck(tFoundIt->second->setVolume(volume_value));
}

void SetChannelPan(int nChannelId, float panValue)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;


	ErrorCheck(tFoundIt->second->setPan(panValue));
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



void ChangeVolumeByDistance(int nChannelId, AudioSource localSource)
{
	float distance = 0;
	float volume = 0;

	distance = sqrt(pow(spatializerData.listener.position.x - localSource.position.x, 2) + pow(spatializerData.listener.position.y - localSource.position.y, 2) + pow(spatializerData.listener.position.z - localSource.position.z, 2));
	if (distance <= spatializerData.min_Sound_Distance)
		volume = 1.2f;
	else if (distance >= spatializerData.max_Sound_Distance)
		volume = 0.0f;
	else
		volume = 1  - ((distance - spatializerData.min_Sound_Distance) / (spatializerData.max_Sound_Distance - spatializerData.min_Sound_Distance));
	
	SetChannelVolume(nChannelId, volume);
	
	return;
}


float ChangePanByOrientation(int nChannelId, AudioSource localSource)
{

	float angle = AngleValue(localSource);

	float panValue = sin(angle);
	

	SetChannelPan(nChannelId, panValue);

	return panValue;
}


void SetListener(Vector3 pos, Vector3 forward, Vector3 up) {
	
	spatializerData.listener.position= pos;
	spatializerData.listener.forward = forward;
	spatializerData.listener.up = up;
}



void SetMinMaxDistance(float min, float max) {
	spatializerData.min_Sound_Distance = min;
	spatializerData.max_Sound_Distance = max;
}


float AngleValue(AudioSource localSource)
{
	Vector3 tempForward;
	tempForward.x = spatializerData.listener.forward.x;
	tempForward.z = spatializerData.listener.forward.z;
	tempForward.y = 0;


	Vector3 side = VectorCrossProduct(spatializerData.listener.up, spatializerData.listener.forward);
	side = VectorNormalize(side);
	//side.y = 0;
	Vector3 difference = VectorSubtract(localSource.position, spatializerData.listener.position);
	//difference.y = 0;



	float x = VectorDotProduct(difference, side);
	float z = VectorDotProduct(difference, spatializerData.listener.forward);
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

	return angleRadians;
}

int SetSources(Vector3* array, int size) //tribalo bi radit, ako slucajno pristupis i-tom elementu koji je veci od sizea, vratit ce neki nasumicni broj
{
	
	spatializerData.numberOfSources = size;

	for (int i = 0; i < size; i++)
	{
		
		spatializerData.mSources[i].position = array[i];
	}
	
	return 0;
}

string GetPathFromChannel(int channelId)
{
	char soundName[256];

	auto tFoundIt = sgpImplementation->mChannels.find(channelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return 0;

	FMOD::Sound* ppSound = NULL;
	ErrorCheck(tFoundIt->second->getCurrentSound(&ppSound));
	ErrorCheck(ppSound->getName(soundName, 256));
	
	string soundString(soundName);
	
	return soundString;
}

int  SpatializeSourcesAndAudio()
{
	for (auto it = spatializerData.mSources.begin(), itEnd = spatializerData.mSources.end(); it != itEnd; ++it)
	{
		ChangeVolumeByDistance(it->first, it->second);
		ChangePanByOrientation(it->first, it->second);
	}
	return 0;
}

/*int SetTest(TestStruct* test, int size)
{
	for (int i = 0; i < size; i++)
	{

	}

}*/