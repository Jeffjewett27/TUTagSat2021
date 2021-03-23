#include "UVSensor.h"
#include "ADC.h"

const int UVA_FN_CODE = 0x11;
const int UVC_FN_CODE = 0x12;
const int UVA_CHANNEL = 6;
const int UVC_CHANNEL = 7;

uint16_t uvaValues[16];
uint16_t uvcValues[16];

uint16_t getUVA() {
  return readAnalog(UVA_CHANNEL);
}  

uint16_t getUVC() {
  return readAnalog(UVC_CHANNEL);
}  

void readUVA(int i){
  uint16_t val = getUVA();
  uvaValues[i] = val;
}
void readUVC(int i) {
  uint16_t val = getUVC();
  uvcValues[i] = val;
}

Packet getUVAPacket(uint8_t iter, uint8_t pc) {
  Packet packet;
  setPacketHeader(&packet, UVA_FN_CODE, iter, pc);
  for (int i = 0; i < PACKET_NUM_2_BYTE; i++) {
    packet.ArrayType.twoByte[i] = uvaValues[i];
  }   
  return packet;
}  
Packet getUVCPacket(uint8_t iter, uint8_t pc) {
  Packet packet;
  setPacketHeader(&packet, UVC_FN_CODE, iter, pc);
  for (int i = 0; i < PACKET_NUM_2_BYTE; i++) {
    packet.ArrayType.twoByte[i] = uvcValues[i];
  }   
  return packet;
}  