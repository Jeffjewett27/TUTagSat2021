#include "simpletools.h"
#include "DataCollection.h"
#include "DataPacketing.h"
#include "PacketQueue.h"
#include "SerialOutput.h"

void initializeSensors();
void initializeDataCollection();
void initializeDataPacketing();
void initializeQueues(PacketQueue *mainQueue, PacketQueue *priorityQueue);

int main() {  
  uint8_t iteration = 0;
  uint8_t packetCount = 0;
  PacketQueue *mainQueue = malloc(sizeof(PacketQueue));
  PacketQueue *priorityQueue = malloc(sizeof(PacketQueue));
  initializeQueues(mainQueue, priorityQueue);
  
  initializeSensors();
  initializeDataCollection();
  initializeDataPacketing();
 
  while(1)
  {
    //collect data for a minute
    minuteLoop();
    //enqueue the data into packets
    packetCount = enqueueAllPackets(mainQueue, iteration, packetCount);
    
    while (getQueueSize(mainQueue) > 4) {pause(10);}
  
    iteration++;
  }  
}

void initializeSensors() {
  //call sensor initialization functions
}  

void initializeDataCollection() {
  //subscribe sensors to data collection
  //subscribeInitialize(&printInit);
  //subscribeEveryTwoSec(&printTwo);
  //subscribeEveryTwoSecAfter(&printTwoAfter);
  //subscribeEveryFourSec(&printFour);
  //subscribeEveryFourSecAfter(&printFourAfter2); 
}

void initializeDataPacketing() {
  //subscribe sensors to data packeting
  //subscribeEachIteration(&test2);
  //subscribeOddIteration(&testOdd1);
  //subscribeEvenIteration(&testEven1);
}  

void initializeQueues(PacketQueue *mainQueue, PacketQueue *priorityQueue) {
  initializeQueue(mainQueue);
  initializeQueue(priorityQueue);
  serialOutputThread(mainQueue, priorityQueue);
}  