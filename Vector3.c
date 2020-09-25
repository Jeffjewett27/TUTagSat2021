/*
 * Author: Joseph van der Harst
 * Modification Date: 9/24/20
*/ 

#include "Vector3.h"
#include <math.h>

float vector3fMagnitude(Vector3f input) {
	return sqrt(input.x*input.x + input.y*input.y + input.z*input.z);
}

Vector3f maxEachDimension(Vector3f inputA, Vector3f inputB) {
	Vector3f give = {
		max(inputA.x, inputB.x),
		max(inputA.y, inputB.y),
		max(inputA.z, inputB.z)
	};
	return give;
}

Vector3f minEachDimension(Vector3f inputA, Vector3f inputB) {
	Vector3f give = {
		min(inputA.x, inputB.x),
		min(inputA.y, inputB.y),
		min(inputA.z, inputB.z)
	};
	return give;
}
