#include "Packet.h"
#include "PacketQueue.h"

uint8_t enqueueAllPackets(PacketQueue *queue, uint8_t iteration, uint8_t packetCount);

void subscribeEachIteration(Packet(*func)(uint8_t iter, uint8_t pc));
void subscribeOddIteration(Packet(*func)(uint8_t iter, uint8_t pc));
void subscribeEvenIteration(Packet(*func)(uint8_t iter, uint8_t pc));