#include "simpletools.h"
#include "DataPacketing.h"

#define NUM_PACKET_FUNCTIONS 25

Packet(*packetFunctions[NUM_PACKET_FUNCTIONS])(uint8_t iter, uint8_t pc);
int packetFunctionsCounter = 0;
uint32_t packetFunctionsHalfMask = 0;
uint32_t packetFunctionsOddMask = 0;

uint8_t enqueueAllPackets(PacketQueue *queue, uint8_t iteration, uint8_t packetCount) {
  uint32_t mask = iteration % 2 == 0 ? packetFunctionsOddMask : ~packetFunctionsOddMask;
  mask &= packetFunctionsHalfMask;
  mask = ~mask;
  //in mask, bits are low if they should be skipped
  for (int i = 0; i < packetFunctionsCounter; i++) {
    //print("mask: %d\n", mask);
    if (mask & 1) {
      Packet packet = packetFunctions[i](iteration, packetCount++);
      enqueue(queue, packet);
    }
    mask >>= 1;
    while(getQueueSize(queue) > 2) { pause(100); }
  }  
  return packetCount;
}

void subscribeEachIteration(Packet(*func)(uint8_t iter, uint8_t pc)) {
  if (packetFunctionsCounter >= NUM_PACKET_FUNCTIONS) {
    return;
  }    
  packetFunctions[packetFunctionsCounter] = func;
  packetFunctionsCounter++;
}  

void subscribeOddIteration(Packet(*func)(uint8_t iter, uint8_t pc)) {
  packetFunctionsHalfMask |= 1 << packetFunctionsCounter;
  packetFunctionsOddMask |= 1 << packetFunctionsCounter;
  subscribeEachIteration(func);
}  

void subscribeEvenIteration(Packet(*func)(uint8_t iter, uint8_t pc)) {
  packetFunctionsHalfMask |= 1 << packetFunctionsCounter;
  subscribeEachIteration(func);
}