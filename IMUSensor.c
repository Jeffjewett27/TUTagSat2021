/*
 * Author: Isaac Wickham and Jeff Jewett
 * Modification Date: 10/6/20
*/

#include "lsm9ds1.h"
#include "IMUSensor.h"
#include "simpletools.h"
#include <math.h>
#include "DataConversion.h"

#define IMU_SCL 8
#define IMU_SDIO 9
#define IMU_CS_AG 5
#define IMU_CS_M 6
#define IMU_EN 27
#define IMU_FN_CODE_BASE 0x40
#define IMU_COMP_FN_CODE_BASE 0x20

int imuInitVerified = 0;

enum imuDataSheetConsts {
  ACCEL_X_ROW = 0, ACCEL_Y_ROW = 1, ACCEL_Z_ROW = 2,
  GYRO_X_ROW = 3 , GYRO_Y_ROW = 4 , GYRO_Z_ROW = 5,
  MAGNE_X_ROW = 6, MAGNE_Y_ROW = 7, MAGNE_Z_ROW = 8,
  IMU_NUM_ROWS = 9, IMU_NUM_COLS = 16
};
float imuSensorDataSheet[IMU_NUM_ROWS][IMU_NUM_COLS];

void imuReadEveryFourSec(int index) {
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
  imu_init(IMU_SCL, IMU_SDIO, IMU_CS_M, IMU_CS_AG);
  high(IMU_EN);
}

Vector3f imuAccelerometerRead() {
  Vector3f acceleration; //G-s
  imu_readAccelCalculated(&(acceleration.x), &(acceleration.y), &(acceleration.z));
  if (!imuInitVerified && acceleration.x < -1000.0) {
    imuInitialize();
    imuInitVerified = 1;
  }    
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

//------Networking------
Packet getDataSheetPacket(uint8_t iter, uint8_t pc, int dataSheetRow) {
  Packet desiredPacket;
  setPacketHeader(&desiredPacket, IMU_FN_CODE_BASE + dataSheetRow, iter, pc);
  for (int index = 0; index < IMU_NUM_COLS; index+=2) {
    desiredPacket.ArrayType.fourByte[index/2] = imuSensorDataSheet[dataSheetRow][index];
  }

  return desiredPacket;
}

Packet getDataSheetCompPacket(uint8_t iter, uint8_t pc, int dataSheetRow) {
  Packet desiredPacket;
  setPacketHeader(&desiredPacket, IMU_COMP_FN_CODE_BASE + dataSheetRow, iter, pc);
  for (int index = 0; index < IMU_NUM_COLS; index++) {
    uint16_t comp = reduceFloat16bit(imuSensorDataSheet[dataSheetRow][index], 1, 5);
    desiredPacket.ArrayType.twoByte[index] = comp;
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

//compressed
Packet getAccelXCompPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetCompPacket(iter, pc, ACCEL_X_ROW);
}
Packet getAccelYCompPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetCompPacket(iter, pc, ACCEL_Y_ROW);
}
Packet getAccelZCompPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetCompPacket(iter, pc, ACCEL_Z_ROW);
}
Packet getGyroXCompPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetCompPacket(iter, pc, GYRO_X_ROW);
}
Packet getGyroYCompPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetCompPacket(iter, pc, GYRO_Y_ROW);
}
Packet getGyroZCompPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetCompPacket(iter, pc, GYRO_Z_ROW);
}
Packet getMagXCompPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetCompPacket(iter, pc, MAGNE_X_ROW);
}
Packet getMagYCompPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetCompPacket(iter, pc, MAGNE_Y_ROW);
}
Packet getMagZCompPacket(uint8_t iter, uint8_t pc) {
  return getDataSheetCompPacket(iter, pc, MAGNE_Z_ROW);
}