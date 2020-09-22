/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

#include "simpletools.h"
#include "TemperatureSensors.h"
#include "IMUSensor.h"
#include "ADC.h"

const uint8_t TEMP2_PIN = 1;

float temperature1Read() {  
  //Temperature from IMU
  return imuTemperatureRead();
}  

uint16_t temperature2Read() {
  uint16_t val = readAnalog(TEMP2_PIN);
  return val;
}  

float temperature2Convert(uint16_t val) {
  float voltage = convertAnalog(val);
  float deltaV = voltage - 0/*1.263*/;
  float converted = deltaV / 0.01 / 1.08789;
  float farh = (converted*9)/5 + 32;
  print("converted: %.2fC, %.2fF\n", converted, farh);
  return converted;
}  