/*
 * Author: Isaac Wickham and Jeff Jewett
 * Modification Date: 10/6/20
*/

#include "lsm9ds1.h"
#include "IMUSensor.h"
#include "simpletools.h"
#include <math.h>

int __pinM;
Vector3i __mBiasRawVector;
unsigned char __settings_mag_scale;

const int IMU_SCL = 0, IMU_SDIO = 1, IMU_CS_AG = 2, IMU_CS_M = 3;

enum imuDataSheetConsts {
  ACCEL_X_ROW = 0, ACCEL_Y_ROW = 1, ACCEL_Z_ROW = 2,
  GYRO_X_ROW = 3 , GYRO_Y_ROW = 4 , GYRO_Z_ROW = 5,
  MAGNE_X_ROW = 6, MAGNE_Y_ROW = 7, MAGNE_Z_ROW = 8,
  IMU_NUM_ROWS = 9, IMU_NUM_COLS = 8
};
float imuSensorDataSheet[IMU_NUM_ROWS][IMU_NUM_COLS];

void imuReadEveryEightSec(int index) {
  Vector3f accelReading = imuAccelerometerRead();
  imuSensorDataSheet[ACCEL_X_ROW][index] = accelReading.x;
  imuSensorDataSheet[ACCEL_Y_ROW][index] = accelReading.y;
  imuSensorDataSheet[ACCEL_Z_ROW][index] = accelReading.z;
  
  Vector3f gyroReading = imuGyroscopeRead();
  imuSensorDataSheet[GYRO_X_ROW][index] = gyroReading.x;
  imuSensorDataSheet[GYRO_Y_ROW][index] = gyroReading.y;
  imuSensorDataSheet[GYRO_Z_ROW][index] = gyroReading.z;

  Vector3f magnetReading = imuMagnetometerRead();
  imuSensorDataSheet[MAGNE_X_ROW][index] = magnetReading.x;
  imuSensorDataSheet[MAGNE_Y_ROW][index] = magnetReading.y;
  imuSensorDataSheet[MAGNE_Z_ROW][index] = magnetReading.z;
}

void imuInitialize() {
  // Success: 0x683D, Failure: 0
  imu_init(IMU_SCL, IMU_SDIO, IMU_CS_AG, IMU_CS_M);
}

Vector3f imuAccelerometerRead() {
  Vector3f acceleration; //G-s
  imu_readAccelCalculated(&(acceleration.x), &(acceleration.y), &(acceleration.z));
  return acceleration;
}  

Vector3f imuGyroscopeRead() {
  Vector3f gyroscope; //Degrees of rotation per second
  imu_readGyroCalculated(&(gyroscope.x), &(gyroscope.y), &(gyroscope.z));
  return gyroscope;
}  

Vector3f imuMagnetometerRead() {
  Vector3f magnet; //Gausses
  imu_readMagCalculated(&(magnet.x), &(magnet.y), &(magnet.z));
  return magnet;
} 

float imuTemperatureRead() {
  float temperature; //Degrees Celsius
  imu_readTempCalculated(&temperature, CELSIUS);
  return temperature;
} 

void imuCalibrateMagnetometerAndAccelerometer() {
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

//------Networking------
Packet getDataSheetPacket(uint8_t iter, uint8_t pc, int dataSheetRow) {
  Packet desiredPacket;
  desiredPacket.iteration = iter;
  desiredPacket.packetsCounter = pc;
  for (int index = 0; index < IMU_NUM_COLS; index++) {
    desiredPacket.ArrayType.fourByte[index] = imuSensorDataSheet[dataSheetRow][index];
  }

  return desiredPacket;
}

Packet getAccelXPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetPacket(iter, pc, ACCEL_X_ROW);
}
Packet getAccelYPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetPacket(iter, pc, ACCEL_Y_ROW);
}
Packet getAccelZPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetPacket(iter, pc, ACCEL_Z_ROW);
}
Packet getGyroXPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetPacket(iter, pc, GYRO_X_ROW);
}
Packet getGyroYPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetPacket(iter, pc, GYRO_Y_ROW);
}
Packet getGyroZPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetPacket(iter, pc, GYRO_Z_ROW);
}
Packet getMagXPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetPacket(iter, pc, MAGNE_X_ROW);
}
Packet getMagYPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetPacket(iter, pc, MAGNE_Y_ROW);
}
Packet getMagZPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetPacket(iter, pc, MAGNE_Z_ROW);
}