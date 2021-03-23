#include "simpletools.h"
#include "DataCollection.h"
#include "DataPacketing.h"
#include "EEPROM.h"
#include "GridEyeSensor.h"
#include "IMUSensor.h"
#include "PacketQueue.h"
#include "PulseCount.h"
#include "SerialOutput.h"
#include "TemperatureSensor.h"
#include "UVSensor.h"
#include "Vector3.h"

uint8_t iteration = 0;
uint8_t packetCount = 0;
PacketQueue *mainQueue;
PacketQueue *priorityQueue;

void initializeSensors(PacketQueue *mainQueue, PacketQueue *priorityQueue);
void initializeDataCollection();
void initializeDataPacketing();
void initializeQueues(PacketQueue *mainQueue, PacketQueue *priorityQueue);

int main() {
  mainQueue = malloc(sizeof(PacketQueue));
  priorityQueue = malloc(sizeof(PacketQueue));
  initializeQueues(mainQueue, priorityQueue);

  initializeSensors(mainQueue, priorityQueue);
  initializeDataCollection();
  initializeDataPacketing();

  while(1)
  {
    //collect data for a minute
    minuteLoop();

    //enqueue the data into packets
    packetCount = enqueueAllPackets(mainQueue, iteration, packetCount);
    
    while (getQueueSize(mainQueue) > 2) {pause(10); }
    iteration++;
  }  
}

void initializeSensors(PacketQueue *mainQueue, PacketQueue *priorityQueue) {
  //call sensor initialization functions
  imuInitialize();
  startLtfThread();
  startRadThread();
  //startGridEyeThread(priorityQueue);
}  

void initializeDataCollection() {
  //subscribe sensors to data collection 
  subscribeEveryFourSec(&imuReadEveryFourSec);
  subscribeEveryFourSec(&readUVA);
  subscribeEveryFourSec(&readUVC);
  subscribeEveryFourSec(&readTemp1);
  subscribeEveryFourSec(&readTemp2);
  subscribeEveryFourSec(&readTemp3);
  
  subscribeInitialize(&initLtf);
  subscribeInitialize(&initRad);
  subscribeEveryFourSec(&lightToFrequencyRead);
  subscribeEveryTwoSec(&radiationRead);
}

void initializeDataPacketing() {
  //subscribe sensors to data packeting
  subscribeEvenIteration(&getAccelXPacket);
  subscribeEvenIteration(&getAccelYPacket);
  subscribeEvenIteration(&getAccelZPacket);
  subscribeEvenIteration(&getGyroXPacket);
  subscribeEvenIteration(&getGyroYPacket);
  subscribeEvenIteration(&getGyroZPacket);
  subscribeEvenIteration(&getMagXPacket);
  subscribeEvenIteration(&getMagYPacket);
  subscribeEvenIteration(&getMagZPacket);

  subscribeOddIteration(&getAccelXCompPacket);
  subscribeOddIteration(&getAccelYCompPacket);
  subscribeOddIteration(&getAccelZCompPacket);
  subscribeOddIteration(&getGyroXCompPacket);
  subscribeOddIteration(&getGyroYCompPacket);
  subscribeOddIteration(&getGyroZCompPacket);
  subscribeOddIteration(&getMagXCompPacket);
  subscribeOddIteration(&getMagYCompPacket);
  subscribeOddIteration(&getMagZCompPacket);
  
  subscribeEachIteration(&getUVAPacket);
  subscribeEachIteration(&getUVCPacket);
  subscribeEachIteration(&getTemp1Packet);
  subscribeEachIteration(&getTemp2Packet);
  subscribeEachIteration(&getTemp3Packet);
  
  subscribeEachIteration(&getRadiationPacket);
  subscribeEachIteration(&getLightToFrequencyPacket);
}  

void initializeQueues(PacketQueue *mainQueue, PacketQueue *priorityQueue) {
  initializeQueue(mainQueue);
  initializeQueue(priorityQueue);
  serialOutputThread(mainQueue, priorityQueue);
}  