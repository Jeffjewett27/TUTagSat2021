/*
 * Author: Joseph van der Harst
 * Modification Date: 10/1/20
*/
#include "simpletools.h"
#include "Packet.h"

uint16_t getUVA();
uint16_t getUVC();

void readUVA(int i);
void readUVC(int i);

Packet getUVAPacket(uint8_t iter, uint8_t pc);
Packet getUVCPacket(uint8_t iter, uint8_t pc);