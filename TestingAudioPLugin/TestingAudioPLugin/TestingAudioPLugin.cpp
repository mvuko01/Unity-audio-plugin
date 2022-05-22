// TestingAudioPLugin.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "MyAudioPlugin/AudioPlugin.h"
#include <iostream>
#include <string>
#include "TestingAudioPLugin.h"




using namespace std;

int main()
{
	
	Vector3 vec = { 0.0f,0.0f,0.0f };


	Vector3 a = { 4.0f,-2.0f, 5.0f };
	Vector3 b = { -1.0f, 3.0f, -6.0f };
	Vector3 c = { 7.0f, -5.0f, 1.0f };

	Vector3 rez = VectorCrossProduct(b, a);
	cout << "a x b: " << rez.x << " " << rez.y << " " << rez.z;


	rez = VectorCrossProduct(b, c);
	cout << "b x c: " << rez.x << " " << rez.y << " " << rez.z;


	rez = VectorCrossProduct(a, c);
	cout << "a x c: " << rez.x << " " << rez.y << " " << rez.z;

	rez = VectorNormalize(a);
	cout << "Normalized a: " << rez.x << " " << rez.y << " " << rez.z<<endl;
	
	AudioSource source;
	AudioListener listener;

	source.position = { 30.0f, 100.0f, -30.0f };
	listener.position = { 30.0f, 0.0f, 0.0f };
	listener.forward = { 0.0f, 0.0f, 1.0f };
	listener.up = { 0.0f, 1.0f, 0.0f };

	float angle = AngleValue(listener, source);

	cout << endl << "Angle is "<< angle;
	//InitAudioEngine();
	//
	//LoadSound((char*)"C:\\Users\\Matej\\Desktop\\test\\singing.wav", true);
	////LoadSound((char*)"C:\\Users\\Matej\\Desktop\\test\\jaguar.wav", false);
	//PlaySounds((char*)"C:\\Users\\Matej\\Desktop\\test\\singing.wav", 1.25f);
	////PlaySounds((char*)"C:\\Users\\Matej\\Desktop\\test\\jaguar.wav", 50);
	//
	//while (1) {
	//	UpdateAudioEngine();
	//}
	
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
	



