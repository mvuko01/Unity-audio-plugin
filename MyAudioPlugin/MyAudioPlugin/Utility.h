#pragma once

#include<math.h>

struct Vector3 {
	float x;
	float y;
	float z;
};


float VectorDotProduct(Vector3 vec1, Vector3 vec2);

Vector3 VectorCrossProduct(Vector3 vec1, Vector3 vec2);

Vector3 VectorSubtract(Vector3 vec1, Vector3 vec2);

Vector3 VectorNormalize(Vector3 vec1);

float VectorMagnitude(Vector3 vec1);

double VectorDistance(Vector3 vec1, Vector3 vec2);