#include "Packet.h"

void startLtfThread();
void ltfThread();
void initLtf();
uint16_t ltfReadReset();
void lightToFrequencyRead(int i);

void startRadThread();
void radThread();
void initRad();
uint8_t radReadReset();
void radiationRead(int i);


Packet getLightToFrequencyPacket(uint8_t iter, uint8_t pc);
Packet getRadiationPacket(uint8_t iter, uint8_t pc);
Packet getPulseCountPacket(uint8_t iter, uint8_t pc);
Packet getPCMoreRadPacket(uint8_t iter, uint8_t pc);