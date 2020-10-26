#include "simpletools.h"
#include "Packet.h"

uint16_t getTemp1();
uint16_t getTemp2();
uint16_t getTemp3();

void readTemp1(int i);
void readTemp2(int i);
void readTemp3(int i);

Packet getTemp1Packet(uint8_t iter, uint8_t pc);
Packet getTemp2Packet(uint8_t iter, uint8_t pc);
Packet getTemp3Packet(uint8_t iter, uint8_t pc);
