/*
 * Author: Isacc Wickham and Jeff Jewett
 * Modification Date: 9/24/20
*/

#ifndef VECTOR3_H
#define VECTOR3_H

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
Vector3f maxEachDimension(Vector3f inputA, Vector3f inputB);
Vector3f minEachDimension(Vector3f inputA, Vector3f inputB);

#endif
