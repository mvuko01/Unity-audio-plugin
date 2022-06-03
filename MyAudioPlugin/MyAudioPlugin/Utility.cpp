#include "Utility.h"


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


float VectorMagnitude(Vector3 vec1)
{
	return sqrtf(vec1.x * vec1.x + vec1.y * vec1.y + vec1.z * vec1.z); 

}

Vector3 VectorNormalize(Vector3 vec1)
{
	Vector3 normalizedVector = { 0,0,0 };
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

double VectorDistance(Vector3 vec1, Vector3 vec2)
{

	return sqrt(pow(vec1.x - vec2.x, 2) + pow(vec1.y - vec2.y, 2) + pow(vec1.z - vec2.z, 2));
 
}