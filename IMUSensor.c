/*
 * Author: Isaac Wickham and Jeff Jewett
 * Modification Date: 9/24/20
*/

#include "lsm9ds1.h"
#include "IMUSensor.h"
#include "simpletools.h"
#include <math.h>

int __pinM;
int __mBiasRaw[3] = {0,0,0};
Vector3i __mBiasRawVector;
unsigned char __settings_mag_scale;

const int IMU_SCL = 0;
const int IMU_SDIO = 1;
const int IMU_CS_AG = 2;
const int IMU_CS_M = 3;

void imuInitialize() {
  // Success: 0x683D, Failure: 0
  imu_init(IMU_SCL, IMU_SDIO, IMU_CS_AG, IMU_CS_M);
}
  
//Unit: g's
Vector3f imuAccelerometerRead() {
  Vector3f acceleration;
  imu_readAccelCalculated(&(acceleration.x), &(acceleration.y), &(acceleration.z));
  return acceleration;
}  

//Unit: Degrees of rotation per second
Vector3f imuGyroscopeRead() {
  Vector3f gyroscope;
  imu_readGyroCalculated(&(gyroscope.x), &(gyroscope.y), &(gyroscope.z));
  return gyroscope;
}  

//Unit: Gauss
Vector3f imuMagnetometerRead() {
  Vector3f magnet;
  imu_readMagCalculated(&(magnet.x), &(magnet.y), &(magnet.z));
  return magnet;
} 

//Unit: Degrees Celsius
float imuTemperatureRead() {
  float temperature;
  imu_readTempCalculated(&temperature, CELSIUS);
  return temperature;
} 

void imuCalibrateMagnetometer() {
  int iterations = 0;
  Vector3i magReading;
  char accelCheck = 0;
  char magRangeCheck = 0;
  Vector3i magReadMin = {0, 0, 0}, magReadMax = {0, 0, 0};
  Vector3f accel;
    
  while(iterations < 128 || (magRangeCheck != 1 && accelCheck != 1)) {
    while (!imu_magAvailable(ALL_AXIS)); // blank loop; grind until available

    imu_readMag(&(magReading.x), &(magReading.y), &(magReading.z));

    magReadMax = maxEachDimensionI(magReadMax, magReading);
    magReadMin = minEachDimensionI(magReadMin, magReading);    

    int minMagDiff = 12000 / ((int) __settings_mag_scale);
    Vector3i magDiff = {
      abs(magReadMax.x - magReadMin.x),
      abs(magReadMax.y - magReadMin.y),
      abs(magReadMax.z - magReadMin.z)
    };
    if (magDiff.x > minMagDiff && magDiff.y > minMagDiff && magDiff.z > minMagDiff) {
      magRangeCheck = 1;
    }

    imu_readAccelCalculated(&(accel.x), &(accel.y), &(accel.z));
    if (vector3fMagnitude(accel) > 0.85) {
      accelCheck = 1;
    }
    
    iterations++;
    if (iterations > 10000) { //has looped for at least 100 seconds
      break;
    }      
    pause(10);
  }

  __mBiasRawVector.x = (magReadMax.x + magReadMin.x) / 2;
  __mBiasRawVector.y = (magReadMax.y + magReadMin.y) / 2;
  __mBiasRawVector.z = (magReadMax.z + magReadMin.z) / 2;

  unsigned char msB, lsB;
  msB = (__mBiasRawVector.x & 0xFF00) >> 8;
  lsB = (__mBiasRawVector.x & 0x00FF);
  imu_SPIwriteByte(__pinM, OFFSET_X_REG_L_M + 0, lsB);
  imu_SPIwriteByte(__pinM, OFFSET_X_REG_H_M + 0, msB);
  
  msB = (__mBiasRawVector.y & 0xFF00) >> 8;
  lsB = (__mBiasRawVector.y & 0x00FF);
  imu_SPIwriteByte(__pinM, OFFSET_X_REG_L_M + 2, lsB);
  imu_SPIwriteByte(__pinM, OFFSET_X_REG_H_M + 2, msB);
  
  msB = (__mBiasRawVector.z & 0xFF00) >> 8;
  lsB = (__mBiasRawVector.z & 0x00FF);
  imu_SPIwriteByte(__pinM, OFFSET_X_REG_L_M + 4, lsB);
  imu_SPIwriteByte(__pinM, OFFSET_X_REG_H_M + 4, msB);
}
