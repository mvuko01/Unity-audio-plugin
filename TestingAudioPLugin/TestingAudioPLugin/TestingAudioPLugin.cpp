// TestingAudioPLugin.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "MyAudioPlugin/AudioPlugin.h"
#include <iostream>
#include <string>





using namespace std;

int main()
{
	
	
	
	/*AudioSource source;
	AudioListener listener;

	source.position = { 30.0f, 0.0f, 0.0f };
	listener.position = { 0.0f, 0.0f, 0.0f };
	listener.forward = { 0.0f, 0.0f, -1.0f };
	listener.up = { 0.0f, 1.0f, 0.0f };

	SetListener(listener.position, listener.forward, listener.up);
	SetSource(source.position);

	float angle = AngleValue();

	float panValue = sin(angle);
	float side = cos(angle);
	
	cout << endl << "Angle is "<< angle;
	cout << endl << "Pan is " << panValue;
	
	*/

	string str;

	

	InitAudioEngine();
	//
	LoadSound((char*)"https://www.youtube.com/watch?v=PkkkKxHJtoo&list=RDPkkkKxHJtoo&start_radio=1&ab_channel=TrapNation", true);
	////LoadSound((char*)"C:\\Users\\Matej\\Desktop\\test\\jaguar.wav", false);
	int id = PlaySounds((char*)"https://www.youtube.com/watch?v=PkkkKxHJtoo&list=RDPkkkKxHJtoo&start_radio=1&ab_channel=TrapNation");

	

	cout << str;
	//SetChannelPan(id, -1);
	////PlaySounds((char*)"C:\\Users\\Matej\\Desktop\\test\\jaguar.wav", 50);
	//
	while (1) {
		UpdateAudioEngine();
	}
	
	/*vec3 side = cross(listener.up, listener.look_at);
	side.normalize(); 
	float x = dot(source.position - listener.position, side); 
	float z = dot(source.position - listener.position, listener.look_at);
	float angle = atan2(x, z);*/  // One way you could compute the panning value from the angle:float pan = sin(angle);


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
	



