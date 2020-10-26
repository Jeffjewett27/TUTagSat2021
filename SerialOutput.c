/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

#include "simpletools.h"
#include "fdserial.h"
#include "SerialOutput.h"

#define BUSY_PIN 24
#define SERIAL_RX 25
#define SERIAL_TX 26

const char ACK[] = {0xAA, 0x05, 0x00};

fdserial *sr;
PacketQueue *packetQueue;
PacketQueue *priorityQueue;
int *thread;

//initializes pins
void initSerial() {
  sr = fdserial_open(SERIAL_RX, SERIAL_TX, 0, 38400);
}  

//starts the serial output thread
void serialOutputThread(PacketQueue *mainQueue, PacketQueue *priorQueue) {
  packetQueue = mainQueue;
  priorityQueue = priorQueue;
  initSerial();
  thread = cog_run(serialOutputLoop, 128);
}  

//forever waits for busy to drop then sends first element of queue
void serialOutputLoop() {
  while(1) {
    while(isSerialBusy()) {
      pause(100);
    }
    //dequeue from priority queue first
    PacketQueue *usedQueue;
    if (!isQueueEmpty(priorityQueue)) {
      usedQueue = priorityQueue;
    } else if (!isQueueEmpty(packetQueue)) {
      usedQueue = packetQueue;
    } else {
      continue;
    }
    Packet packet = peekQueue(usedQueue);
    int attempt = 0;
    do {
        outputPacket(&packet);
    } while (!isSerialACK() && attempt++ < 2);
    dequeue(usedQueue);
    pause(1000);
  }  
}  

//outputs a packet
void outputPacket(Packet *packet) {
  fdserial_rxFlush(sr);
  fdserial_txFlush(sr);

  //header bytes
  fdserial_txChar(sr, 0x50);
  fdserial_txChar(sr, 0x50);
  fdserial_txChar(sr, 0x50);

  fdserial_txChar(sr, packet->fnCode);
  fdserial_txChar(sr, packet->iteration);
  fdserial_txChar(sr, packet->packetsCounter);
  
  for(int i=0; i<32; i++) {
    fdserial_txChar(sr, packet->ArrayType.oneByte[i]);
  }  
}

//determines if signal is ACK
int isSerialACK() {
  int numACKBytes = 3;
  char response[numACKBytes];
  for (int i = 0; i < numACKBytes; ++i) {
    int safetyEscape = 0;
    while(fdserial_rxCount(sr)==0) {
      if (safetyEscape++ > 1500) {
        //ensure we don't get caught in an infinite loop. 1500 iterations * 10ms/iter = 15s. 
        //It should not take anywhere near that long or something is seriously wrong
        return 0;
      }       
      pause(10);
    }
    response[i] = fdserial_rxChar(sr);
  }
  int val = 1; //note: safer comparison
  for (int i = 0; i < numACKBytes; ++i) {
    if (ACK[i] != response[i]) {
      val = 0;
    }      
  }    
  return val;
}  

//checks the busy pin
int isSerialBusy() {
  return input(BUSY_PIN);
}