/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

#include "simpletools.h"
#include "fdserial.h"
#include "SerialOutput.h"
#include "Pins.h"

//#define BUSY_PIN 24

const char ACK[] = {0xAA, 0x05, 0x00};

fdserial *sr;
PacketQueue *packetQueue;
int *thread;

//initializes pins
void initSerial() {
  sr = fdserial_open(SERIAL_RX, SERIAL_TX, FDSERIAL_MODE_OPENDRAIN_TX, 38400);
}  

//starts the serial output thread
void serialOutputThread(PacketQueue *mainQueue) {
  packetQueue = mainQueue;
  initSerial();
  thread = cog_run(serialOutputLoop, 128);
}  

//forever waits for busy to drop then sends first element of queue
void serialOutputLoop() {
  while(1) {
    //wait until there is a packet to send
    while(isQueueEmpty(packetQueue)) { pause(100); }
    //signal that were's ready to send
    high(RTS_PIN);
    //wait until it's ready to receive
    while(!input(CTS_PIN)) { pause(100); }
    //try to send packet
    Packet packet = peekQueue(packetQueue);
    int attempt = 0;
    do {
        outputPacket(&packet);
    } while (!isSerialACK() && attempt++ < 2);
    //cleanup
    dequeue(packetQueue);
    low(RTS_PIN);
    pause(100);
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
  //fdserial_txChar(sr, packet->iteration);
  fdserial_txChar(sr, packet->packetsCounter);
  
  for(int i = 0; i < PACKET_NUM_1_BYTE; i++) {
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
  int val = 1;
  for (int i = 0; i < numACKBytes; ++i) {
    if (ACK[i] != response[i]) {
      val = 0;
    }      
  }    
  return val;
}  

//checks the busy pin
/*int isSerialBusy() {
  return input(BUSY_PIN);
}*/