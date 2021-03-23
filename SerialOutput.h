/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

#ifndef SERIAL_OUTPUT
#define SERIAL_OUTPUT

#include "PacketQueue.h"

//starts the serial output thread
void serialOutputThread(PacketQueue *mainQueue);

//forever waits for busy to drop then sends first element of queue
void serialOutputLoop();

//initializes pins
void initSerial();

//outputs a packet
void outputPacket(Packet *packet);

//waits and returns whether response is ack
int isSerialACK();

//is busy
//int isSerialBusy();
#endif