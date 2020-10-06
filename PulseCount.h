#include "Packet.h"

void startPulseCountThread();
void stopPulseCountThread();
void initPulseCount_ltf();
void pulseCountThread();
uint16_t lightToFrequency_read_reset();
uint8_t radiation_read_reset();
void lightToFrequency_read(int i);
void radiation_read(int i);

Packet getLightToFrequencyPacket(uint8_t iter, uint8_t pc);
Packet getRadiationPacket(uint8_t iter, uint8_t pc);