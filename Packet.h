#ifndef PACKET_H
#define PACKET_H

#include "simpletools.h"

const int PACKET_NUM_1_BYTE = 29;
const int PACKET_NUM_2_BYTE = 14;
const int PACKET_NUM_4_BYTE = 7;

//A packet which will be transmitted
struct Packet {
  uint8_t fnCode;
  union {
    uint8_t oneByte[32];
    uint16_t twoByte[16];
    float fourByte[8];
  } ArrayType;
  uint8_t iteration;
  uint8_t packetsCounter;
};

typedef struct Packet Packet;

#endif

void setPacketHeader(Packet *packet, uint8_t fnCode, uint8_t iteration, uint8_t packetsCounter);

void makePacketDataZero(Packet *packet);