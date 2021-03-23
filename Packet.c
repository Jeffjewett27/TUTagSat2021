#include "Packet.h"

void setPacketHeader(Packet *packet, uint8_t fnCode, uint8_t iteration, uint8_t packetsCounter) {
  packet->fnCode = fnCode;
  packet->iteration = iteration;
  packet->packetsCounter = packetsCounter;
}

void makePacketDataZero(Packet *packet) {
  for (int i = 0; i < PACKET_NUM_1_BYTE; i++) {
    packet->ArrayType.oneByte[i] = 0;
  }    
}  