// Write C++ code here.
//
// Do not forget to dynamically load the C++ library into your application.
//
// For instance,
//
// In MainActivity.java:
//    static {
//       System.loadLibrary("MyAndroidAudioPlugin");
//    }
//
// Or, in MainActivity.kt:
//    companion object {
//      init {
//         System.loadLibrary("MyAndroidAudioPlugin")
//      }
//    }

#include <jni.h>
#include <iostream>
#include <iostream>
#include <fmod_errors.h>
#include <fmod_common.h>
#include <thread>
#include "MyAndroidAudioPlugin.h"


using namespace std;
using namespace FMOD;


Implementation* sgpImplementation = nullptr;
SpatializerData spatializerData;



Implementation::Implementation() {
    system = nullptr;
    ErrorCheck(System_Create(&system));
    RESULT res = ErrorCheck(system->init(10, FMOD_INIT_NORMAL, 0));
    cout << res;
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
        return SOUND_ALREADY_LOADED;


    //definiramo flagove zvuka
    FMOD_MODE eMode = FMOD_DEFAULT;
    eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    eMode |= FMOD_2D;


    FMOD::Sound* sound = nullptr;
    ErrorCheck(sgpImplementation->system->createSound(pstrSoundPath, eMode, nullptr, &sound));

    if (sound) {

        sgpImplementation->mSounds[strSoundPath] = sound;
        return OK;
    }

    return SOUNDLOAD_FAILED;

}




int UnLoadSound(char* pstrSoundPath)
{
    string strSoundPath(pstrSoundPath);

    auto tFoundIt = sgpImplementation->mSounds.find(strSoundPath);
    if (tFoundIt == sgpImplementation->mSounds.end())
        return SOUND_NOT_FOUND;

    ErrorCheck(tFoundIt->second->release());
    sgpImplementation->mSounds.erase(tFoundIt);

    return OK;
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
            return SOUND_NOT_FOUND;
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
    return PLAYSOUND_ERROR;
}

int ReturnNumOfSounds() //funkcija za testiranjes
{

    int n = sgpImplementation->mSounds.size();
    return n;
}

int ReturnNumOfChannels() //funkcija za testiranjes
{

    int n = sgpImplementation->mChannels.size();
    return n;
}


RESULT SetChannelVolume(int nChannelId, float volume_value)
{
    auto tFoundIt = sgpImplementation->mChannels.find(nChannelId); //ovo se moze stavit u funkciju pošto se ponavlja često
    if (tFoundIt == sgpImplementation->mChannels.end())
        return CHANNEL_NOT_FOUND;



    return ErrorCheck(tFoundIt->second->setVolume(volume_value));
}

RESULT SetChannelPan(int nChannelId, float panValue)
{
    auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
    if (tFoundIt == sgpImplementation->mChannels.end())
        return CHANNEL_NOT_FOUND;


    return ErrorCheck(tFoundIt->second->setPan(panValue));
}


int ChangeVolumeByDistance(int nChannelId, AudioSource currentSource)
{
    double distance = 0;
    float volume = 0;

    distance = VectorDistance(spatializerData.listener.position, currentSource.position);
    if (distance <= currentSource.min_Sound_Distance)
        volume = 1.2f;
    else if (distance >= currentSource.max_Sound_Distance)
        volume = 0.0f;
    else
        volume = 1  - ((distance - currentSource.min_Sound_Distance) / (currentSource.max_Sound_Distance - currentSource.min_Sound_Distance));


    return SetChannelVolume(nChannelId, volume);

}


int ChangePanByOrientation(int nChannelId, AudioSource currentSource)
{

    float angle = AngleValue(currentSource);
    float panValue = sin(angle);

    return SetChannelPan(nChannelId, panValue);

}


int SetListener(Vector3 pos, Vector3 forward, Vector3 up) {

    spatializerData.listener.position= pos;
    spatializerData.listener.forward = forward;
    spatializerData.listener.up = up;

    return OK;
}

int SetSources(AudioSource* sourceArray, int size) //tribalo bi radit, ako slucajno pristupis i-tom elementu koji je veci od sizea, vratit ce neki nasumicni broj
{

    spatializerData.numberOfSources = size;

    for (int i = 0; i < size; i++)
    {
        spatializerData.mSources[i].position = sourceArray[i].position;

        if (sourceArray[i].min_Sound_Distance > 0 && sourceArray[i].min_Sound_Distance < sourceArray[i].max_Sound_Distance)
        {
            spatializerData.mSources[i].min_Sound_Distance = sourceArray[i].min_Sound_Distance;

        }
        if (sourceArray[i].max_Sound_Distance > 0 && sourceArray[i].min_Sound_Distance < sourceArray[i].max_Sound_Distance)
        {
            spatializerData.mSources[i].max_Sound_Distance = sourceArray[i].max_Sound_Distance;

        }

    }

    return OK;
}


float AngleValue(AudioSource currentSource)
{
    Vector3 tempForward;
    tempForward.x = spatializerData.listener.forward.x;
    tempForward.z = spatializerData.listener.forward.z;
    tempForward.y = 0;


    Vector3 side = VectorCrossProduct(spatializerData.listener.up, spatializerData.listener.forward);
    side = VectorNormalize(side);

    Vector3 difference = VectorSubtract(currentSource.position, spatializerData.listener.position);


    float x = VectorDotProduct(difference, side);
    float z = VectorDotProduct(difference, spatializerData.listener.forward);
    float angleRadians = atan2(x, z);
    float angleDegrees = angleRadians * (180.0 / 3.14);

    return angleRadians;
}


int  SpatializeSourcesAndAudio()
{
    for (auto it = spatializerData.mSources.begin(), itEnd = spatializerData.mSources.end(); it != itEnd; ++it)
    {
        ChangeVolumeByDistance(it->first, it->second);
        ChangePanByOrientation(it->first, it->second);
    }
    return OK;
}


RESULT ErrorCheck(FMOD_RESULT result) {

    if (result != FMOD_OK) {

        cout << "FMOD ERROR " << result << endl;
        return FMOD_ERROR;

    }
    return OK;
}