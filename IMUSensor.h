/*
 * Author: Isaac Wickham and Jeff Jewett
 * Modification Date: 5/10/20
*/

#include "Vector3.h"

Vector3 imuAccelerometerRead();
Vector3 imuMagnetometerRead();
Vector3 imuGyroscopeRead();
float imuTemperatureRead();
void imuInitialize(); 
void imuCalibrateMagnetometer();