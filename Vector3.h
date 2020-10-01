/*
 * Author: Isacc Wickham and Jeff Jewett
 * Modification Date: 10/1/20
*/

#ifndef VECTOR3_H
#define VECTOR3_H

#ifndef MAX
	#define MAX(a,b) ((a > b) ? a : b)
#endif

#ifndef MIN
	#define MIN(a,b) ((a < b) ? a : b)
#endif

//defines a 3-vector returned by the IMU
struct Vector3f {
  float x;
  float y;
  float z; 
};

struct Vector3i {
  int x;
  int y;
  int z;
};

typedef struct Vector3f Vector3f;
typedef struct Vector3i Vector3i;

float vector3fMagnitude(Vector3f input);
Vector3f maxEachDimensionF(Vector3f inputA, Vector3f inputB);
Vector3f minEachDimensionF(Vector3f inputA, Vector3f inputB);

float vector3iMagnitude(Vector3i input);
Vector3i maxEachDimensionI(Vector3i inputA, Vector3i inputB);
Vector3i minEachDimensionI(Vector3i inputA, Vector3i inputB);

#endif