#include "simpletools.h"
#include "DataPacketing.h"

#define NUM_PACKET_FUNCTIONS 40

Packet(*packetFunctions[NUM_PACKET_FUNCTIONS])(uint8_t iter, uint8_t pc);
int packetFunctionsCounter = 0;
uint32_t packetFunctionsHalfMask = 0;
uint32_t packetFunctionsOddMask = 0;

uint8_t enqueueAllPackets(PacketQueue *queue, uint8_t iteration, uint8_t packetCount) {
  //print("masks: %d, %d\n", packetFunctionsHalfMask, packetFunctionsOddMask);
  uint32_t mask = iteration % 2 == 0 ? packetFunctionsOddMask : ~packetFunctionsOddMask;
  mask &= packetFunctionsHalfMask;
  mask = ~mask;
  //in mask, bits are low if they should be skipped
  for (int i = 0; i < packetFunctionsCounter; i++) {
    //print("mask: %d\n", mask);
    if (mask & 1) {
      Packet packet = packetFunctions[i](iteration, packetCount++);
      //setPacketCount(packetsCounter); 
      enqueue(queue, packet);
    }
    mask >>= 1;
  }  
  return packetCount;
}

void subscribeEachIteration(Packet(*func)(uint8_t iter, uint8_t pc)) {
  if (packetFunctionsCounter >= NUM_PACKET_FUNCTIONS) {
    print("error, packet func overflow\n");
    return;
  }    
  packetFunctions[packetFunctionsCounter] = func;
  packetFunctionsCounter++;
}  

void subscribeOddIteration(Packet(*func)(uint8_t iter, uint8_t pc)) {
  if (packetFunctionsCounter >= NUM_PACKET_FUNCTIONS) {
    print("error, packet func odd overflow\n");
    return;
  }  
  packetFunctionsHalfMask |= 1 << packetFunctionsCounter;
  packetFunctionsOddMask |= 1 << packetFunctionsCounter;
  subscribeEachIteration(func);
}  

void subscribeEvenIteration(Packet(*func)(uint8_t iter, uint8_t pc)) {
  if (packetFunctionsCounter >= NUM_PACKET_FUNCTIONS) {
    print("error, packet func even overflow\n");
    return;
  }    
  packetFunctionsHalfMask |= 1 << packetFunctionsCounter;
  subscribeEachIteration(func);
}