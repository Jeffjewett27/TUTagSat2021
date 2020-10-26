#include "simpletools.h"
#include "DataCollection.h"

#define NUM_DATA_INITIALIZERS 2
#define NUM_DATA_CLEANUPS 2
#define NUM_DATA_TWO_SEC 4
#define NUM_DATA_FOUR_SEC 8
#define NUM_DATA_EIGHT_SEC 4

void(*dataInitializers[NUM_DATA_INITIALIZERS])();
void(*dataCleanups[NUM_DATA_CLEANUPS])();
void(*dataTwoSec[NUM_DATA_TWO_SEC])(int i);
void(*dataFourSec[NUM_DATA_FOUR_SEC])(int i);
void(*dataEightSec[NUM_DATA_EIGHT_SEC])(int i);
int dataInitCounter = 0;
int dataCleanCounter = 0;
int dataTwoSecCounter = 0;
int dataFourSecCounter = 0;
int dataEightSecCounter = 0;
uint16_t dataTwoSecMask = 0;
uint16_t dataFourSecMask = 0;



void minuteLoop() {
  callEachAction(dataInitializers, dataInitCounter);
  
  for (int i = 0; i < 32; i++) {
    callEachSubscriber(dataTwoSec, dataTwoSecCounter, i, dataTwoSecMask);
    if (i % 2 == 0) {
      callEachSubscriber(dataFourSec, dataFourSecCounter, i / 2, dataFourSecMask);
    }      
    if (i % 4 == 0) {
      callEachSubscriber(dataEightSec, dataEightSecCounter, i / 4, 0);
    }      
    pause(2000);
    callEachSubscriber(dataTwoSec, dataTwoSecCounter, i, ~dataTwoSecMask);
    if (i % 2 == 0) {
      callEachSubscriber(dataFourSec, dataFourSecCounter, i / 2, ~dataFourSecMask);
    }  
  }
  callEachAction(dataCleanups, dataCleanCounter);
}  

void subscribeInitialize(void(*func)()) {
  if (dataInitCounter >= NUM_DATA_INITIALIZERS) {
    return;
  }    
  dataInitializers[dataInitCounter] = func;
  dataInitCounter++;
}  

void subscribeCleanup(void(*func)()) {
  if (dataCleanCounter >= NUM_DATA_CLEANUPS) {
    return;
  }    
  dataCleanups[dataCleanCounter] = func;
  dataCleanCounter++;
} 

void subscribeEveryTwoSec(void(*func)(int i)) {
  if (dataTwoSecCounter >= NUM_DATA_TWO_SEC) {
    return;
  }    
  dataTwoSec[dataTwoSecCounter] = func;
  dataTwoSecCounter++;
} 
 
void subscribeEveryFourSec(void(*func)(int i)) {
  if (dataFourSecCounter >= NUM_DATA_FOUR_SEC) {
    return;
  }    
  dataFourSec[dataFourSecCounter] = func;
  dataFourSecCounter++;
}

void subscribeEveryEightSec(void(*func)(int i)) {
  if (dataEightSecCounter >= NUM_DATA_EIGHT_SEC) {
    return;
  }    
  dataEightSec[dataEightSecCounter] = func;
  dataEightSecCounter++;
}

void subscribeEveryTwoSecAfter(void(*func)(int i)) {
  dataTwoSecMask |= (1 << dataTwoSecCounter);
  subscribeEveryTwoSec(func);
}  

void subscribeEveryFourSecAfter(void(*func)(int i)) {
  dataFourSecMask |= (1 << dataFourSecCounter);
  subscribeEveryFourSec(func);
}  

void callEachAction(void(**actions)(), int num) {
  for (int i = 0; i < num; i++) {
    actions[i]();
  }    
}  

void callEachSubscriber(void(**subscribers)(int iter), int num, int val, uint16_t mask) {
  for (int i = 0; i < num; i++) {
    if (mask % 2 == 0) {
      subscribers[i](val);
    }    
    mask >>= 1;
  }  
}  