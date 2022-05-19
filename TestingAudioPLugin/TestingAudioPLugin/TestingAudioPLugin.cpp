// TestingAudioPLugin.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "MyAudioPlugin/AudioPlugin.h"
#include <iostream>
#include <string>




using namespace std;

int main()
{
	
	Vector3 vec = { 0.0f,0.0f,0.0f };

	InitAudioEngine();
	
	LoadSound((char*)"C:\\Users\\Matej\\Desktop\\test\\singing.wav", true);
	//LoadSound((char*)"C:\\Users\\Matej\\Desktop\\test\\jaguar.wav", false);
	PlaySounds((char*)"C:\\Users\\Matej\\Desktop\\test\\singing.wav", 1.25f);
	//PlaySounds((char*)"C:\\Users\\Matej\\Desktop\\test\\jaguar.wav", 50);
	
	while (1) {
		UpdateAudioEngine();
	}
	

	/*AudioPositionVectors source;
	AudioPositionVectors source2;
	AudioPositionVectors listener;

	source.position = { 95.0f, 0.0f, 0.0f };
	source.velocity = { 0.0f, 0.0f, 0.0f };

	source2.position = { 0.0f, 0.0f, 0.0f };
	source2.velocity = { 0.0f, 0.0f, 0.0f };

	listener.position = { 0.0f, 0.0f, 0.0f };
	listener.velocity = { 0.0f, 0.0f, 0.0f };
	listener.forward = { 0.0f, 0.0f, 1.0f };
	listener.up = { 0.0f, 1.0f, 0.0f };

	Spatialize(source, listener, (char*)"C:\\Users\\Matej\\Desktop\\test\\singing.wav", source2, (char*)"C:\\Users\\Matej\\Desktop\\test\\jaguar.wav");*/

	
};
	



