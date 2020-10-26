#include "TemperatureSensor.h"
#include "ADC.h"

#define TEMP1_FN_CODE 0x13
#define TEMP2_FN_CODE 0x14
#define TEMP3_FN_CODE 0x15
#define TEMP1_CHANNEL 3
#define TEMP2_CHANNEL 4
#define TEMP3_CHANNEL 5

uint16_t temp1Values[16];
uint16_t temp2Values[16];
uint16_t temp3Values[16];

uint16_t getTemp1() {
  return readAnalog(TEMP1_CHANNEL);
}  

uint16_t getTemp2() {
  return readAnalog(TEMP2_CHANNEL);
}  

uint16_t getTemp3() {
  return readAnalog(TEMP3_CHANNEL);
}  

void readTemp1(int i){
  uint16_t val = getTemp1();
  temp1Values[i] = val;
}

void readTemp2(int i) {
  uint16_t val = getTemp2();
  temp2Values[i] = val;
}

void readTemp3(int i) {
  uint16_t val = getTemp3();
  temp3Values[i] = val;
}

Packet getTemp1Packet(uint8_t iter, uint8_t pc) {
  Packet packet;
  setPacketHeader(&packet, TEMP1_FN_CODE, iter, pc);
  for (int i = 0; i < 16; i++) {
    packet.ArrayType.twoByte[i] = temp1Values[i];
  }   
  return packet;
}  
Packet getTemp2Packet(uint8_t iter, uint8_t pc) {
  Packet packet;
  setPacketHeader(&packet, TEMP2_FN_CODE, iter, pc);
  for (int i = 0; i < 16; i++) {
    packet.ArrayType.twoByte[i] = temp2Values[i];
  }   
  return packet;
}  

Packet getTemp3Packet(uint8_t iter, uint8_t pc) {
  Packet packet;
  setPacketHeader(&packet, TEMP3_FN_CODE, iter, pc);
  for (int i = 0; i < 16; i++) {
    packet.ArrayType.twoByte[i] = temp3Values[i];
  }   
  return packet;
}  