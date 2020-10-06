/*
 * Author: Isaac Wickham and Jeff Jewett
 * Modification Date: 10/6/20
*/

#ifndef IMU_SENSOR
#define IMU_SENSOR

#include "Vector3.h"
#include "Packet.h"

void imuInitialize();
void imuReadEveryEightSec(int index);

Vector3f imuAccelerometerRead();
Vector3f imuMagnetometerRead();
Vector3f imuGyroscopeRead();
float imuTemperatureRead();

void imuCalibrateMagnetometer();

//Registered networking functions
Packet getAccelXPacket(uint8_t iter, uint8_t pc);
Packet getAccelYPacket(uint8_t iter, uint8_t pc);
Packet getAccelZPacket(uint8_t iter, uint8_t pc);
Packet getGyroXPacket(uint8_t iter, uint8_t pc);
Packet getGyroYPacket(uint8_t iter, uint8_t pc);
Packet getGyroZPacket(uint8_t iter, uint8_t pc);
Packet getMagXPacket(uint8_t iter, uint8_t pc);
Packet getMagYPacket(uint8_t iter, uint8_t pc);
Packet getMagZPacket(uint8_t iter, uint8_t pc);

#endif
