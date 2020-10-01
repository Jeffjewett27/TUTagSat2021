/*
 * Author: Joseph van der Harst
 * Modification Date: 10/1/20
*/ 

#include "Vector3.h"
#include <math.h>

float vector3fMagnitude(Vector3f input) {
	return sqrt(input.x*input.x + input.y*input.y + input.z*input.z);
}

Vector3f maxEachDimensionF(Vector3f inputA, Vector3f inputB) {
	Vector3f give = {
		MAX(inputA.x, inputB.x),
		MAX(inputA.y, inputB.y),
		MAX(inputA.z, inputB.z)
	};
	return give;
}

Vector3f minEachDimensionF(Vector3f inputA, Vector3f inputB) {
	Vector3f give = {
		MIN(inputA.x, inputB.x),
		MIN(inputA.y, inputB.y),
		MIN(inputA.z, inputB.z)
	};
	return give;
}

float vector3iMagnitude(Vector3i input) {
	return sqrt(input.x*input.x + input.y*input.y + input.z*input.z);
}

Vector3i maxEachDimensionI(Vector3i inputA, Vector3i inputB) {
	Vector3i give = {
		MAX(inputA.x, inputB.x),
		MAX(inputA.y, inputB.y),
		MAX(inputA.z, inputB.z)
	};
	return give;
}

Vector3i minEachDimensionI(Vector3i inputA, Vector3i inputB) {
	Vector3i give = {
		MIN(inputA.x, inputB.x),
		MIN(inputA.y, inputB.y),
		MIN(inputA.z, inputB.z)
	};
	return give;
}