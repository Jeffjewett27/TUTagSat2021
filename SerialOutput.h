/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

#include "PacketQueue.h"

//starts the serial output thread
void serialOutputThread(PacketQueue *pQueue);

//forever waits for busy to drop then sends first element of queue
void serialOutputLoop();

//initializes pins
void initSerial();

//outputs a packet
void outputPacket(char* packet);

//waits and returns whether response is ack
int isSerialACK();

//is busy
int isSerialBusy();