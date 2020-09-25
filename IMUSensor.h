/*
 * Author: Isaac Wickham and Jeff Jewett
 * Modification Date: 5/10/20
*/

#ifndef IMU_SENSOR
#define IMU_SENSOR

#include "Vector3.h"

void imuInitialize(); 

Vector3f imuAccelerometerRead();
Vector3f imuMagnetometerRead();
Vector3f imuGyroscopeRead();
float imuTemperatureRead();

void imuCalibrateMagnetometer();

#endif
